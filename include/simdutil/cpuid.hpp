#ifndef SIMDUTIL_CPUID_HPP
#define SIMDUTIL_CPUID_HPP

#include <algorithm>
#include <array>
#include <string>
#include <type_traits>
#include <utility>

#if defined(__GNUC__)
#  include <cpuid.h>
#elif defined(_MSC_VER)
#  include <intrin.h>
#endif


namespace simdutil
{


template<
  typename T,
  typename std::enable_if<std::is_same<T, int*>::value, std::nullptr_t>::type = nullptr
>
static inline void
cpuid(T cpuinfo, int eax) noexcept
{
#if defined(__GNUC__)
  ::__cpuid(eax, cpuinfo[0], cpuinfo[1], cpuinfo[2], cpuinfo[3]);
#elif defined(_MSC_VER)
  ::__cpuid(cpuinfo, eax);
#endif  // defined(__GNUC__)
}

template<
  typename T,
  typename std::enable_if<std::is_same<T, int*>::value, std::nullptr_t>::type = nullptr
>
static inline void
cpuid(T cpuInfo, int eax) noexcept
{
#if defined(__GNUC__)
  ::__cpuid(eax, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif defined(_MSC_VER)
  ::__cpuid(cpuInfo, eax);
#endif  // defined(__GNUC__)
}

template<std::size_t kSize>
static inline void
cpuid(int (&cpuInfo)[kSize], int eax) noexcept
{
  static_assert(kSize >= 4, "CPU info array size must be four or more");

  cpuid(&cpuInfo[0], eax);
}

template<std::size_t kSize>
static inline void
cpuid(std::array<int, kSize>& cpuInfo, int eax) noexcept
{
  static_assert(kSize >= 4, "CPU info array size must be four or more");

  cpuid(cpuInfo.data(), eax);
}

template<
  typename T,
  typename std::enable_if<std::is_same<T, int*>::value, std::nullptr_t>::type = nullptr
>
static inline void
cpuidex(T cpuInfo, int eax, int ecx) noexcept
{
#if defined(__GNUC__)
  ::__cpuid_count(eax, ecx, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif defined(_MSC_VER)
  ::__cpuidex(cpuInfo, eax, ecx);
#endif  // defined(__GNUC__)
}

template<std::size_t kSize>
static inline void
cpuidex(int (&cpuInfo)[kSize], int eax, int ecx) noexcept
{
  static_assert(kSize >= 4, "[util::cpuidex] CPU info array size must be four or more");

  cpuidex(&cpuInfo[0], eax, ecx);
}

template<std::size_t kSize>
static inline void
cpuidex(std::array<int, kSize>& cpuInfo, int eax, int ecx) noexcept
{
  static_assert(kSize >= 4, "[util::cpuidex] CPU info array size must be four or more");

  cpuidex(cpuInfo.data(), eax, ecx);
}

static inline bool
cpuidBit(int eax, int index, int nBit) noexcept
{
    std::array<int, 4> cpuinfo;
    cpuid(cpuinfo, eax);
    return (cpuinfo[index] & (1 << nBit)) != 0;
}

static inline bool
cpuidexBit(int eax, int ecx, int index, int nBit) noexcept
{
  std::array<int, 4> cpuinfo;
  cpuidex(cpuinfo, eax, ecx);
  return (cpuinfo[index] & (1 << nBit)) != 0;
}

static inline bool
isMmxAvailable() noexcept
{
  return cpuidBit(1, 3, 23);
}

static inline bool
isSseAvailable() noexcept
{
  return cpuidBit(1, 3, 25);
}

static inline bool
isSse2Available() noexcept
{
  return cpuidBit(1, 3, 26);
}

static inline bool
isSse3Available() noexcept
{
  return cpuidBit(1, 2, 0);
}

static inline bool
isSsse3Available() noexcept
{
  return cpuidBit(1, 2, 9);
}

static inline bool
isSse41Available() noexcept
{
  return cpuidBit(1, 2, 19);
}

static inline bool
isSse42Available() noexcept
{
  return cpuidBit(1, 2, 20);
}

static inline bool
isSse4aAvailable() noexcept
{
  std::array<int, 4> cpuinfo;
  cpuid(cpuinfo, 0x80000000);
  if (static_cast<unsigned int>(cpuinfo[0]) < 0x80000001U) {
    return false;
  }
  return cpuidBit(0x80000001, 2, 6);
}

static inline bool
isAvxAvailable() noexcept
{
  return cpuidBit(1, 2, 28);
}

static inline bool
isAvx2Available() noexcept
{
  return cpuidBit(7, 1, 5);
}

static inline bool
isFmaAvailable() noexcept
{
  return cpuidBit(1, 2, 12);
}

static inline bool
isAvx512FAvailable() noexcept
{
  return cpuidBit(7, 1, 16);
}

static inline bool
isAvx512BwAvailable() noexcept
{
  return cpuidBit(7, 1, 30);
}

static inline bool
isAvx512CdAvailable() noexcept
{
  return cpuidBit(7, 1, 28);
}

static inline bool
isAvx512DqAvailable() noexcept
{
  return cpuidBit(7, 1, 17);
}

static inline bool
isAvx512ErAvailable() noexcept
{
  return cpuidBit(7, 1, 27);
}

static inline bool
isAvx512Ifma52Available() noexcept
{
  return cpuidBit(7, 1, 21);
}

static inline bool
isAvx512PfAvailable() noexcept
{
  return cpuidBit(7, 1, 26);
}

static inline bool
isAvx512VlAvailable() noexcept
{
  return cpuidBit(7, 1, 31);
}

static inline bool
isAvx512_4fmapsAvailable() noexcept
{
  return cpuidBit(7, 3, 2);
}

static inline bool
isAvx512_4vnniwAvailable() noexcept
{
  return cpuidBit(7, 3, 3);
}

static inline bool
isAvx512BitalgAvailable() noexcept
{
  return cpuidBit(7, 2, 12);
}

static inline bool
isAvx512VpopcntdqAvailable() noexcept
{
  return cpuidBit(7, 2, 14);
}

static inline bool
isAvx512VbmiAvailable() noexcept
{
  return cpuidBit(7, 2, 1);
}

static inline bool
isAvx512Vbmi2Available() noexcept
{
  return cpuidBit(7, 2, 6);
}

static inline bool
isAvx512VnniAvailable() noexcept
{
  return cpuidBit(7, 2, 6);
}


//// 以下はおまけ

template<
  typename T,
  typename std::enable_if<std::is_same<T, char*>::value, std::nullptr_t>::type = nullptr
>
static inline void
copyCpuVendorId(T vendorId) noexcept
{
  std::array<int, 4> cpuinfo;
  cpuid(cpuinfo, 0);

  const auto p = reinterpret_cast<int*>(vendorId);
  p[0] = cpuinfo[1];
  p[1] = cpuinfo[3];
  p[2] = cpuinfo[2];
}

template<std::size_t kSize>
static inline void
copyCpuVendorId(char (&vendorId)[kSize]) noexcept
{
  static_assert(kSize >= 12, "CPU vendor ID array size must be 12 or more");

  copyCpuVendorId(vendorId.data());
}

template<std::size_t kSize>
static inline void
copyCpuVendorId(std::array<char, kSize>& vendorId) noexcept
{
  static_assert(kSize >= 12, "CPU vendor ID array size must be 12 or more");

  copyCpuVendorId(vendorId.data());
}

static inline std::string
getCpuVendorId() noexcept
{
  std::array<char, 32> vendorId;
  std::fill(std::begin(vendorId), std::end(vendorId), '\0');

  copyCpuVendorId(vendorId);

  return std::string{ vendorId.data() };
}

template<
  typename T,
  typename std::enable_if<std::is_same<T, char*>::value, std::nullptr_t>::type = nullptr
>
static inline void
copyCpuBrandString(T dst) noexcept
{
  std::array<int, 4> cpuinfo;

  cpuid(cpuinfo, 0x80000000);
  if (cpuinfo[0] >= 0x80000004) {
    dst[0] = '\0';
    return;
  }

  const auto p = reinterpret_cast<int*>(dst);

  cpuid(cpuinfo, 0x80000002);
  std::copy(std::begin(cpuinfo), std::end(cpuinfo), &p[0]);

  cpuid(cpuinfo, 0x80000003);
  std::copy(std::begin(cpuinfo), std::end(cpuinfo), &p[cpuinfo.size()]);

  cpuid(cpuinfo, 0x80000004);
  std::copy(std::begin(cpuinfo), std::end(cpuinfo), &p[cpuinfo.size() * 2]);
}

template<std::size_t kSize>
static inline void
copyCpuBrandString(char (&vendorId)[kSize]) noexcept
{
  static_assert(kSize >= 64, "CPU brand string array size must be 64 or more");

  copyCpuBrandString(vendorId);
}

template<std::size_t kSize>
static inline void
copyCpuBrandString(std::array<char, kSize>& vendorId) noexcept
{
  static_assert(kSize >= 64, "CPU brand string array size must be 64 or more");

  copyCpuBrandString(vendorId.data());
}

static inline std::string
getCpuBrandString() noexcept
{
  std::array<char, 64> brandStringArray;
  std::fill(std::begin(brandStringArray), std::end(brandStringArray), '\0');

  copyCpuVendorId(brandStringArray);

  return std::string{ brandStringArray.data() };
}

static inline void
getL2CacheSize(int& cacheSize, int& cacheLineSize) noexcept
{
  std::array<int, 4> cpuInfo;
  cpuid(cpuInfo, 0x80000000);
  if (cpuInfo[0] < 0x80000006u) {
    cacheSize = -1;
    cacheLineSize = -1;
    return;
  }
  cpuid(cpuInfo, 0x80000006);
  cacheSize = (cpuInfo[2] & 0xffff0000u) >> 6;
  cacheLineSize = (cpuInfo[2] & 0xffu);
}


}  // namespace simdutil


#endif  // SIMDUTIL_CPUID_HPP
