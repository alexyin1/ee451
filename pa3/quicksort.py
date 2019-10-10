import random
import queue
import copy


def swap(idx1, idx2, arr):
    temp = copy.copy(arr[idx2])
    arr[idx2] = arr[idx1]
    arr[idx1] = temp
    return arr

q = queue.Queue()
for j in range(100):
    nums = [random.randint(0,50) for i in range(20)]
    orig = copy.copy(nums)
    start = 0
    end = len(nums)
    q.put((start, end))
    # print("before", nums)
    while q.empty() is False:
        start, end = q.get()
        pivot = nums[end-1]
        l_end = start-1
        if start < end-1:
            for i in range(start, end, 1):
                if nums[i] < pivot:
                    l_end+=1
                    swap(l_end, i, nums)
            swap(l_end+1, end-1, nums)
            # print("putting", (start, l_end+1))
            q.put((start, l_end+1)) # left first
            # print("putting", (l_end+2, end))
            q.put((l_end+2, end))
    print("after", nums)
    orig.sort()
    assert orig == nums
