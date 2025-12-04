#ifndef __ERDP_HEAP_HPP__
#define __ERDP_HEAP_HPP__
#include <cstddef>
#include <stdint.h>
#include <cstdio>
#include "erdp_assert.h"
#include "erdp_config.h"
#define HEAP4_DEBUG 0

#ifndef ERDP_ENABLE_RTOS
class Heap4
{
public:
    struct Stats
    {
        size_t total_size;
        size_t free_size;
        size_t min_free_size;
        size_t alloc_count;
        size_t max_alloc_count;
    };

    Heap4() = default;
    explicit Heap4(void *heap_area, size_t heap_size) noexcept { init(heap_area, heap_size); }
    Heap4(const Heap4 &) = delete;
    Heap4 &operator=(const Heap4 &) = delete;

    void init(void *heap_area, size_t heap_size) noexcept
    {
        uint8_t *heap = static_cast<uint8_t *>(heap_area);
        size_t aligned_size = align_up(heap_size);
        erdp_assert(aligned_size >= MIN_BLOCK_SIZE);
        heap_start_ = reinterpret_cast<BlockHeader *>(heap);
        heap_start_->size = aligned_size;
        heap_start_->next = nullptr;
#ifdef HEAP4_DEBUG
        heap_start_->in_use = false;
#endif
        free_list_ = heap_start_;
        heap_end_ = reinterpret_cast<BlockHeader *>(heap + aligned_size);
        free_size_ = aligned_size;
        min_free_size_ = aligned_size;
        alloc_count_ = 0;
        max_alloc_count_ = 0;
    }

    void *allocate(size_t size) noexcept
    {
        if (size == 0)
            return nullptr;
        size_t total_size = align_up(size + sizeof(BlockHeader));
        if (total_size < MIN_BLOCK_SIZE)
            total_size = MIN_BLOCK_SIZE;
        BlockHeader *prev = nullptr;
        BlockHeader *curr = free_list_;
        while (curr)
        {
            if (curr->size >= total_size)
                break;
            prev = curr;
            curr = curr->next;
        }
        if (!curr)
            return nullptr;
        if (curr->size >= total_size + MIN_BLOCK_SIZE)
        {
            // 拆分
            BlockHeader *next_block = reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t *>(curr) + total_size);
            next_block->size = curr->size - total_size;
            next_block->next = curr->next;
#ifdef HEAP4_DEBUG
            next_block->in_use = false;
#endif
            curr->size = total_size;
            if (prev)
                prev->next = next_block;
            else
                free_list_ = next_block;
        }
        else
        {
            // 不拆分
            if (prev)
                prev->next = curr->next;
            else
                free_list_ = curr->next;
        }
#ifdef HEAP4_DEBUG
        curr->in_use = true;
#endif
        free_size_ -= curr->size;
        if (free_size_ < min_free_size_)
            min_free_size_ = free_size_;
        alloc_count_++;
        if (alloc_count_ > max_alloc_count_)
            max_alloc_count_ = alloc_count_;
        return reinterpret_cast<void *>(curr + 1);
    }

    void deallocate(void *ptr) noexcept
    {
        if (!ptr)
            return;
        BlockHeader *block = reinterpret_cast<BlockHeader *>(ptr) - 1;
        erdp_assert(block >= heap_start_ && block < heap_end_);
        erdp_assert(block->size >= sizeof(BlockHeader));
#ifdef HEAP4_DEBUG
        erdp_assert(block->in_use);
        block->in_use = false;
#endif
        // 检查free_list无重复
        BlockHeader *iter = free_list_;
        while (iter)
        {
            erdp_assert(iter != block);
            iter = iter->next;
        }
        insert_and_coalesce(block);
        if (alloc_count_ > 0)
            alloc_count_--;
    }

    Stats get_stats() const noexcept
    {
        Stats s;
        s.total_size = reinterpret_cast<const uint8_t *>(heap_end_) - reinterpret_cast<const uint8_t *>(heap_start_);
        s.free_size = free_size_;
        s.min_free_size = min_free_size_;
        s.alloc_count = alloc_count_;
        s.max_alloc_count = max_alloc_count_;
        return s;
    }

    bool validate() noexcept
    {
        // 检查free_list无环
        BlockHeader *slow = free_list_;
        BlockHeader *fast = free_list_ ? free_list_->next : nullptr;
        while (fast && fast->next)
        {
            erdp_assert(slow != fast);
            slow = slow->next;
            fast = fast->next->next;
        }
        // 检查free_list顺序和区间
        size_t free_sum = 0;
        BlockHeader *prev = nullptr;
        BlockHeader *curr = free_list_;
        while (curr)
        {
            erdp_assert(curr >= heap_start_ && curr < heap_end_);
            erdp_assert(curr->size >= MIN_BLOCK_SIZE);
            if (prev)
                erdp_assert(prev < curr);
            free_sum += curr->size;
            prev = curr;
            curr = curr->next;
        }
        free_size_ = free_sum;
        erdp_assert(free_sum <= reinterpret_cast<uint8_t *>(heap_end_) - reinterpret_cast<uint8_t *>(heap_start_));
        // 检查所有块
        BlockHeader *block = heap_start_;
        while (block < heap_end_)
        {
            erdp_assert(block->size >= sizeof(BlockHeader));
            block = reinterpret_cast<BlockHeader *>(reinterpret_cast<uint8_t *>(block) + block->size);
        }
        return block == heap_end_;
    }

private:
    struct BlockHeader
    {
        BlockHeader *next;
        size_t size;
#ifdef HEAP4_DEBUG
        bool in_use;
#endif
    };
    static constexpr size_t ALIGNMENT = 8;
    static constexpr size_t MIN_BLOCK_SIZE = sizeof(BlockHeader) * 2;
    static size_t align_up(size_t sz) noexcept { return (sz + ALIGNMENT - 1) & ~(ALIGNMENT - 1); }

    void insert_and_coalesce(BlockHeader *block) noexcept
    {
        // 插入到free_list，按地址排序
        BlockHeader *prev = nullptr;
        BlockHeader *curr = free_list_;
        while (curr && curr < block)
        {
            prev = curr;
            curr = curr->next;
        }
        // 前向合并
        if (curr && reinterpret_cast<uint8_t *>(block) + block->size == reinterpret_cast<uint8_t *>(curr))
        {
            block->size += curr->size;
            block->next = curr->next;
        }
        else
        {
            block->next = curr;
        }
        // 后向合并
        if (prev && reinterpret_cast<uint8_t *>(prev) + prev->size == reinterpret_cast<uint8_t *>(block))
        {
            prev->size += block->size;
            prev->next = block->next;
        }
        else
        {
            if (prev)
                prev->next = block;
            else
                free_list_ = block;
        }
// Debug输出
#if HEAP4_DEBUG
        BlockHeader *tmp = free_list_;
        printf("[FreeList]: ");
        while (tmp)
        {
            printf("%p(%zu) -> ", tmp, tmp->size);
            tmp = tmp->next;
        }
        printf("NULL\n");
#endif
    }

    BlockHeader *heap_start_ = nullptr;
    BlockHeader *heap_end_ = nullptr;
    BlockHeader *free_list_ = nullptr;
    size_t free_size_ = 0;
    size_t min_free_size_ = 0;
    size_t alloc_count_ = 0;
    size_t max_alloc_count_ = 0;
};
#endif // !ERDP_ENABLE_RTOS
#endif // __ERDP_HEAP_HPP__

