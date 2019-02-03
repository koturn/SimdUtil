// InstructionSet.cpp Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about
// CPU extended instruction set support.

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>


#if defined(__GNUC__)
#  include <cpuid.h>
#elif defined(_MSC_VER)
#  include <intrin.h>
#endif


static inline void
cpuid(int* cpuInfo, int eax) noexcept
{
#if defined(__GNUC__)
  __cpuid(eax, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif defined(_MSC_VER)
  __cpuid(cpuInfo, eax);
#endif  // defined(__GNUC__)
}


template<std::size_t kSize>
static inline void
cpuid(std::array<int, kSize>& cpuInfo, int eax) noexcept
{
  static_assert(kSize >= 4, "CPU info array size must be four or more");
  cpuid(cpuInfo.data(), eax);
}


static inline void
cpuidex(int* cpuInfo, int eax, int ecx) noexcept
{
#if defined(__GNUC__)
  __cpuid_count(eax, ecx, cpuInfo[0], cpuInfo[1], cpuInfo[2], cpuInfo[3]);
#elif defined(_MSC_VER)
  __cpuidex(cpuInfo, eax, ecx);
#endif  // defined(__GNUC__)
}


template<std::size_t kSize>
static inline void
cpuidex(std::array<int, kSize>& cpuInfo, int eax, int ecx) noexcept
{
  static_assert(kSize >= 4, "CPU info array size must be four or more");
  cpuidex(cpuInfo.data(), eax, ecx);
}


class InstructionSet
{
  // forward declarations
  class InstructionSet_Internal;

public:
  // getters
  static std::string Vendor() noexcept { return CPU_Rep.vendor_; }
  static std::string Brand() noexcept { return CPU_Rep.brand_; }
  static bool SSE3() noexcept { return CPU_Rep.f_1_ECX_[0]; }
  static bool PCLMULQDQ() noexcept { return CPU_Rep.f_1_ECX_[1]; }
  static bool MONITOR() noexcept { return CPU_Rep.f_1_ECX_[3]; }
  static bool SSSE3() noexcept { return CPU_Rep.f_1_ECX_[9]; }
  static bool FMA() noexcept { return CPU_Rep.f_1_ECX_[12]; }
  static bool CMPXCHG16B() noexcept { return CPU_Rep.f_1_ECX_[13]; }
  static bool SSE41() noexcept { return CPU_Rep.f_1_ECX_[19]; }
  static bool SSE42() noexcept { return CPU_Rep.f_1_ECX_[20]; }
  static bool MOVBE() noexcept { return CPU_Rep.f_1_ECX_[22]; }
  static bool POPCNT() noexcept { return CPU_Rep.f_1_ECX_[23]; }
  static bool AES() noexcept { return CPU_Rep.f_1_ECX_[25]; }
  static bool XSAVE() noexcept { return CPU_Rep.f_1_ECX_[26]; }
  static bool OSXSAVE() noexcept { return CPU_Rep.f_1_ECX_[27]; }
  static bool AVX() noexcept { return CPU_Rep.f_1_ECX_[28]; }
  static bool F16C() noexcept { return CPU_Rep.f_1_ECX_[29]; }
  static bool RDRAND() noexcept { return CPU_Rep.f_1_ECX_[30]; }
  static bool MSR() noexcept { return CPU_Rep.f_1_EDX_[5]; }
  static bool CX8() noexcept { return CPU_Rep.f_1_EDX_[8]; }
  static bool SEP() noexcept { return CPU_Rep.f_1_EDX_[11]; }
  static bool CMOV() noexcept { return CPU_Rep.f_1_EDX_[15]; }
  static bool CLFSH() noexcept { return CPU_Rep.f_1_EDX_[19]; }
  static bool MMX() noexcept { return CPU_Rep.f_1_EDX_[23]; }
  static bool FXSR() noexcept { return CPU_Rep.f_1_EDX_[24]; }
  static bool SSE() noexcept { return CPU_Rep.f_1_EDX_[25]; }
  static bool SSE2() noexcept { return CPU_Rep.f_1_EDX_[26]; }
  static bool FSGSBASE() noexcept { return CPU_Rep.f_7_EBX_[0]; }
  static bool BMI1() noexcept { return CPU_Rep.f_7_EBX_[3]; }
  static bool HLE() noexcept { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
  static bool AVX2() noexcept { return CPU_Rep.f_7_EBX_[5]; }
  static bool BMI2() noexcept { return CPU_Rep.f_7_EBX_[8]; }
  static bool ERMS() noexcept { return CPU_Rep.f_7_EBX_[9]; }
  static bool INVPCID() noexcept { return CPU_Rep.f_7_EBX_[10]; }
  static bool RTM() noexcept { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
  static bool AVX512F() noexcept { return CPU_Rep.f_7_EBX_[16]; }
  static bool AVX512DQ() noexcept { return CPU_Rep.f_7_EBX_[17]; }
  static bool RDSEED() noexcept { return CPU_Rep.f_7_EBX_[18]; }
  static bool ADX() noexcept { return CPU_Rep.f_7_EBX_[19]; }
  static bool AVX512IFMA() noexcept { return CPU_Rep.f_7_EBX_[21]; }
  static bool AVX512PF() noexcept { return CPU_Rep.f_7_EBX_[26]; }
  static bool AVX512ER() noexcept { return CPU_Rep.f_7_EBX_[27]; }
  static bool AVX512CD() noexcept { return CPU_Rep.f_7_EBX_[28]; }
  static bool SHA() noexcept { return CPU_Rep.f_7_EBX_[29]; }
  static bool AVX512BW() noexcept { return CPU_Rep.f_7_EBX_[30]; }
  static bool AVX512VL() noexcept { return CPU_Rep.f_7_EBX_[31]; }
  static bool PREFETCHWT1() noexcept { return CPU_Rep.f_7_ECX_[0]; }
  static bool AVX512VBMI() noexcept { return CPU_Rep.f_7_ECX_[1]; }
  static bool AVX512VBMI2() noexcept { return CPU_Rep.f_7_ECX_[6]; }
  static bool AVX512VNNI() noexcept { return CPU_Rep.f_7_ECX_[11]; }
  static bool AVX512BITALG() noexcept { return CPU_Rep.f_7_ECX_[12]; }
  static bool AVX512VPOPCNTDQ() noexcept { return CPU_Rep.f_7_ECX_[14]; }
  static bool AVX512_4VNNIW() noexcept { return CPU_Rep.f_7_EDX_[2]; }
  static bool AVX512_4FMAPS() noexcept { return CPU_Rep.f_7_EDX_[3]; }
  static bool LAHF() noexcept { return CPU_Rep.f_81_ECX_[0]; }
  static bool LZCNT() noexcept { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
  static bool ABM() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
  static bool SSE4a() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
  static bool XOP() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
  static bool TBM() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }
  static bool SYSCALL() noexcept { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
  static bool MMXEXT() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
  static bool RDTSCP() noexcept { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
  static bool _3DNOWEXT() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
  static bool _3DNOW() noexcept { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
  static const InstructionSet_Internal CPU_Rep;

  class InstructionSet_Internal
  {
  public:
    InstructionSet_Internal()
      : nIds_{0}
      , nExIds_{0}
      , vendor_{}
      , brand_{}
      , isIntel_{false}
      , isAMD_{false}
      , f_1_ECX_{0}
      , f_1_EDX_{0}
      , f_7_EBX_{0}
      , f_7_ECX_{0}
      , f_7_EDX_{0}
      , f_81_ECX_{0}
      , f_81_EDX_{0}
      , data_{}
      , extdata_{}
    {
      std::array<int, 4> cpui;

      // Calling __cpuid with 0x0 as the function_id argument
      // gets the number of the highest valid function ID.
      cpuid(cpui, 0);
      nIds_ = cpui[0];

      for (int i = 0; i <= nIds_; ++i) {
        cpuidex(cpui, i, 0);
        data_.push_back(cpui);
      }

      // Capture vendor string
      std::array<char, 0x20> vendor;
      std::fill(std::begin(vendor), std::end(vendor), '\0');
      *reinterpret_cast<int*>(&vendor[0]) = data_[0][1];
      *reinterpret_cast<int*>(&vendor[4]) = data_[0][3];
      *reinterpret_cast<int*>(&vendor[8]) = data_[0][2];
      vendor_ = std::string(vendor.data());
      if (vendor_ == "GenuineIntel") {
        isIntel_ = true;
      } else if (vendor_ == "AuthenticAMD") {
        isAMD_ = true;
      }

      // load bitset with flags for function 0x00000001
      if (nIds_ >= 1) {
        f_1_ECX_ = data_[1][2];
        f_1_EDX_ = data_[1][3];
      }

      // load bitset with flags for function 0x00000007
      if (nIds_ >= 7) {
        f_7_EBX_ = data_[7][1];
        f_7_ECX_ = data_[7][2];
        f_7_EDX_ = data_[7][3];
      }

      // Calling __cpuid with 0x80000000 as the function_id argument
      // gets the number of the highest valid extended ID.
      cpuid(cpui, 0x80000000);
      nExIds_ = cpui[0];

      std::array<char, 0x40> brand;
      std::fill(std::begin(brand), std::end(brand), '\0');

      for (int i = 0x80000000; i <= nExIds_; ++i) {
        cpuidex(cpui, i, 0);
        extdata_.push_back(cpui);
      }

      // load bitset with flags for function 0x80000001
      if (static_cast<unsigned int>(nExIds_) >= 0x80000001) {
        f_81_ECX_ = extdata_[1][2];
        f_81_EDX_ = extdata_[1][3];
      }

      // Interpret CPU brand string if reported
      if (static_cast<unsigned int>(nExIds_) >= 0x80000004) {
        std::copy(std::cbegin(extdata_[2]), std::cend(extdata_[2]), reinterpret_cast<int*>(&brand[0]));
        std::copy(std::cbegin(extdata_[3]), std::cend(extdata_[3]), reinterpret_cast<int*>(&brand[0] + sizeof(extdata_[0])));
        std::copy(std::cbegin(extdata_[4]), std::cend(extdata_[4]), reinterpret_cast<int*>(&brand[0] + sizeof(extdata_[0]) * 2));
        brand_ = std::string(brand.data());
      }
    };

    int nIds_;
    int nExIds_;
    std::string vendor_;
    std::string brand_;
    bool isIntel_;
    bool isAMD_;
    std::bitset<32> f_1_ECX_;
    std::bitset<32> f_1_EDX_;
    std::bitset<32> f_7_EBX_;
    std::bitset<32> f_7_ECX_;
    std::bitset<32> f_7_EDX_;
    std::bitset<32> f_81_ECX_;
    std::bitset<32> f_81_EDX_;
    std::vector<std::array<int, 4>> data_;
    std::vector<std::array<int, 4>> extdata_;
  };  // class InstructionSet_Internal
};  // class InstructionSet


// Initialize static member data
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


// Print out supported instruction set extensions
int
main()
{
  auto &outstream = std::cout;
  auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
    outstream << isa_feature
              << (is_supported ? " supported" : " not supported")
              << std::endl;
  };

  std::cout << InstructionSet::Vendor() << std::endl;
  std::cout << InstructionSet::Brand() << std::endl;

  support_message("3DNOW", InstructionSet::_3DNOW());
  support_message("3DNOWEXT", InstructionSet::_3DNOWEXT());
  support_message("ABM", InstructionSet::ABM());
  support_message("ADX", InstructionSet::ADX());
  support_message("AES", InstructionSet::AES());
  support_message("AVX", InstructionSet::AVX());
  support_message("AVX2", InstructionSet::AVX2());
  support_message("AVX512CD", InstructionSet::AVX512CD());
  support_message("AVX512ER", InstructionSet::AVX512ER());
  support_message("AVX512F", InstructionSet::AVX512F());
  support_message("AVX512DQ", InstructionSet::AVX512DQ());
  support_message("AVX512IFMA", InstructionSet::AVX512IFMA());
  support_message("AVX512PF", InstructionSet::AVX512PF());
  support_message("AVX512BW", InstructionSet::AVX512BW());
  support_message("AVX512VL", InstructionSet::AVX512VL());
  support_message("AVX512VBMI", InstructionSet::AVX512VBMI());
  support_message("AVX512VBMI2", InstructionSet::AVX512VBMI2());
  support_message("AVX512VNNI", InstructionSet::AVX512VNNI());
  support_message("AVX512BITALG", InstructionSet::AVX512BITALG());
  support_message("AVX512VPOPCNTDQ", InstructionSet::AVX512VPOPCNTDQ());
  support_message("AVX512_4VNNIW", InstructionSet::AVX512_4VNNIW());
  support_message("AVX512_4FMAPS", InstructionSet::AVX512_4FMAPS());
  support_message("BMI1", InstructionSet::BMI1());
  support_message("BMI2", InstructionSet::BMI2());
  support_message("CLFSH", InstructionSet::CLFSH());
  support_message("CMPXCHG16B", InstructionSet::CMPXCHG16B());
  support_message("CX8", InstructionSet::CX8());
  support_message("ERMS", InstructionSet::ERMS());
  support_message("F16C", InstructionSet::F16C());
  support_message("FMA", InstructionSet::FMA());
  support_message("FSGSBASE", InstructionSet::FSGSBASE());
  support_message("FXSR", InstructionSet::FXSR());
  support_message("HLE", InstructionSet::HLE());
  support_message("INVPCID", InstructionSet::INVPCID());
  support_message("LAHF", InstructionSet::LAHF());
  support_message("LZCNT", InstructionSet::LZCNT());
  support_message("MMX", InstructionSet::MMX());
  support_message("MMXEXT", InstructionSet::MMXEXT());
  support_message("MONITOR", InstructionSet::MONITOR());
  support_message("MOVBE", InstructionSet::MOVBE());
  support_message("MSR", InstructionSet::MSR());
  support_message("OSXSAVE", InstructionSet::OSXSAVE());
  support_message("PCLMULQDQ", InstructionSet::PCLMULQDQ());
  support_message("POPCNT", InstructionSet::POPCNT());
  support_message("PREFETCHWT1", InstructionSet::PREFETCHWT1());
  support_message("RDRAND", InstructionSet::RDRAND());
  support_message("RDSEED", InstructionSet::RDSEED());
  support_message("RDTSCP", InstructionSet::RDTSCP());
  support_message("RTM", InstructionSet::RTM());
  support_message("SEP", InstructionSet::SEP());
  support_message("SHA", InstructionSet::SHA());
  support_message("SSE", InstructionSet::SSE());
  support_message("SSE2", InstructionSet::SSE2());
  support_message("SSE3", InstructionSet::SSE3());
  support_message("SSE4.1", InstructionSet::SSE41());
  support_message("SSE4.2", InstructionSet::SSE42());
  support_message("SSE4a", InstructionSet::SSE4a());
  support_message("SSSE3", InstructionSet::SSSE3());
  support_message("SYSCALL", InstructionSet::SYSCALL());
  support_message("TBM", InstructionSet::TBM());
  support_message("XOP", InstructionSet::XOP());
  support_message("XSAVE", InstructionSet::XSAVE());
}
