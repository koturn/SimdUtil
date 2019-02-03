#ifndef SIMDUTIL_ALLOCATOR_HPP
#define SIMDUTIL_ALLOCATOR_HPP


#include <iterator>
#include <stdexcept>
#include <type_traits>


#if defined(_MSC_VER) || defined(__MINGW32__)
#  include <malloc.h>
#else
#  include <cstdlib>
#endif  // defined(_MSC_VER) || defined(__MINGW32__)


#if __cplusplus >= 201103L \
    || defined(_MSC_VER) && (_MSC_VER > 1800 || (_MSC_VER == 1800 && _MSC_FULL_VER == 180021114))
#  define SIMDUTIL_ALLOCATOR_ALIGNAS(N) alignas(T)
#elif defined(__GNUC__)
#  define SIMDUTIL_ALLOCATOR_ALIGNAS(N) __attribute__((aligned(n)))
#elif defined(_MSC_VER)
#  define SIMDUTIL_ALLOCATOR_ALIGNAS(N) __declspec(align(n))
#endif

#if __cplusplus >= 201103L \
    || defined(_MSC_VER) && (_MSC_VER > 1800 || (_MSC_VER == 1800 && _MSC_FULL_VER == 180021114))
#  define SIMDUTIL_ALLOCATOR_ALIGNOF(T) alignof(T)
#elif defined(__GNUC__)
#  define SIMDUTIL_ALLOCATOR_ALIGNOF(T) __alignof__(T)
#elif defined(_MSC_VER)
#  define SIMDUTIL_ALLOCATOR_ALIGNOF(T) __alignof(T)
#endif


namespace simdutil
{
/*!
 * @brief Get alignment of specified type
 *
 * This function is a polyfill of "alignof" (C++11)
 *
 * @return Alignment of specified type
 */
template<typename T>
static inline constexpr std::size_t
alignOf() noexcept
{
  return SIMDUTIL_ALLOCATOR_ALIGNOF(T);
}

/*!
 * @brief Allocate aligned memory
 * @param [in] size       Memory size
 * @param [in] alignment  Alignment (Must be power of 2)
 * @return  Allocated aligned memory
 */
template<typename T = void>
static inline T*
alignedMalloc(std::size_t nBytes, std::size_t alignment) noexcept
{
#if defined(__cplusplus) && __cplusplus >= 201703L
  return reinterpret_cast<T*>(std::aligned_alloc(alignment, nBytes));
#elif defined(_MSC_VER) || defined(__MINGW32__)
  return reinterpret_cast<T*>(::_aligned_malloc(nBytes, alignment));
#else
  void* p;
  return reinterpret_cast<T*>(::posix_memalign(&p, alignment, nBytes) == 0 ? p : nullptr);
#endif  // defined(_MSC_VER) || defined(__MINGW32__)
}

/*!
 * @brief Allocate aligned memory
 * @param [in] size       Memory size
 * @param [in] alignment  Alignment (Must be power of 2)
 * @return  Allocated aligned memory
 */
template<typename T = std::uint8_t>
static inline T*
alignedAllocArray(std::size_t size, std::size_t alignment = alingOf<T>()) noexcept
{
  return alignedMalloc<T>(size * sizeof(T), alignment);
}

/*!
 * @brief Free aligned memory
 * @param [in] ptr  Aligned memory
 */
static inline void
alignedFree(void* ptr) noexcept
{
#if defined(__cplusplus) && __cplusplus >= 201703L
  std::free(ptr);
#elif defined(_MSC_VER) || defined(__MINGW32__)
  ::_aligned_free(ptr);
#else
  std::free(ptr);
#endif  // defined(_MSC_VER) || defined(__MINGW32__)
}

/*!
 * @brief Custom deleter for std::unique_ptr or std::shared_ptr
 */
struct AlignedDeleter
{
  /*!
   * @brief Operator: () for delete action
   * @param [in,out] p  A pointer to alignd memory
   */
  void
  operator()(void* p) const noexcept
  {
    alignedFree(p);
  }
};  // struct AlignedDeleter


/*！
 * @brief Custom allocator for STL. This class allocates aligned memory.
 */
template<
  typename T,
  std::size_t kAlignment = sizeof(T)
>
class AlignedAllocator
{
public:
  //! Element type
  using value_type = T;
  //! Size type
  using size_type = std::size_t;
  //! Pointer type
  using pointer = typename std::add_pointer<value_type>::type;
  //! Const pointer type
  using const_pointer = typename std::add_pointer<const value_type>::type;

  /*!
   * @brief Definition for rebinded allocator.
   *
   * This definition is neccessary because type parameter of this class is not only one.
   */
  template<class U>
  struct rebind
  {
    //! Rebinded allocator type
    using other = AlignedAllocator<U, kAlignment>;
  };

  /*!
   * Default constructor
   */
  AlignedAllocator() noexcept
  {}

  /*!
   * Converting copy constructor
   */
  template<typename U>
  AlignedAllocator(const AlignedAllocator<U, kAlignment>&) noexcept
  {}

  /*!
   * @brief Allocate memory block for STL container
   * @param [in] n     Number of elements to allocate
   * @param [in] hint  Hint parameter (unused)
   * @return  Pointer to aligned memroy
   */
  pointer
  allocate(size_type n, const_pointer /* hint */ = nullptr) const
  {
    auto p = alignedAllocArray<value_type>(n, kAlignment);
    if (p == nullptr) {
      throw std::bad_alloc{};
    }
    return p;
  }

  /*!
   * @brief Free allocated STL memory
   * @param [in,out] p  Pointer to dynamic allocated memory
   * @param [in]     n  Number of elements (unused)
   */
  void
  deallocate(pointer p, size_type /* n */) const noexcept
  {
    alignedFree(p);
  }
};  // class AlignedAllocator


template<
  typename T,
  std::size_t kAlignment1,
  typename U,
  std::size_t kAlignment2
>
static inline bool
operator==(const AlignedAllocator<T, kAlignment1>&, const AlignedAllocator<U, kAlignment2>&) noexcept
{
  return kAlignment1 == kAlignment2;
}


template<
  typename T,
  std::size_t kAlignment1,
  typename U,
  std::size_t kAlignment2
>
static inline bool
operator!=(const AlignedAllocator<T, kAlignment1>& lhs, const AlignedAllocator<U, kAlignment2>& rhs) noexcept
{
  return !(lhs == rhs);
}


}  // namespace simdutil


#endif  // SIMDUTIL_ALLOCATOR_HPP

