
`grep -n "template " $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`

152:template <typename T> struct iterator_traits<fmt::basic_appender<T>> {
177:// integer formatter template instantiations to just one by only using the
186:template <typename Char, typename Traits, typename Allocator>
248:template <typename Char> using std_string_view = std::basic_string_view<Char>;
250:template <typename Char> struct std_string_view {
255:template <typename Char, Char... C> struct string_literal {
262:template <typename Char, Char... C>
267:template <typename To, typename From, FMT_ENABLE_IF(sizeof(To) == sizeof(From))>
304:  template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
407:template <typename T> constexpr auto max_value() -> T {
410:template <typename T> constexpr auto num_bits() -> int {
414:template <> constexpr auto num_bits<native_int128>() -> int { return 128; }
415:template <> constexpr auto num_bits<native_uint128>() -> int { return 128; }
416:template <> constexpr auto num_bits<uint128>() -> int { return 128; }
420:template <typename To, typename From, FMT_ENABLE_IF(sizeof(To) > sizeof(From))>
437:template <typename UInt>
470:template <typename OutputIt,
484:template <typename T>
492:template <typename Iterator>
497:template <typename OutputIt>
501:template <typename T, typename OutputIt>
505:template <typename T> FMT_CONSTEXPR auto to_pointer(T*& ptr, size_t n) -> T* {
510:template <typename T>
520:template <typename OutputIt,
529:template <typename Iterator>
536:template <typename OutputIt, typename Size, typename T>
542:template <typename T, typename Size>
551:template <typename T, typename V, typename OutputIt>
556:template <typename OutChar, typename InputIt, typename OutputIt>
620:template <typename F>
677:template <typename T> struct is_integral : std::is_integral<T> {};
678:template <> struct is_integral<native_int128> : std::true_type {};
679:template <> struct is_integral<uint128_t> : std::true_type {};
681:template <typename T>
686:template <typename T>
708:template <typename T> using is_float128 = std::is_same<T, float128>;
710:template <typename T> struct is_floating_point : std::is_floating_point<T> {};
711:template <> struct is_floating_point<float128> : std::true_type {};
713:template <typename T, bool = is_floating_point<T>::value>
716:template <typename T> struct is_fast_float<T, false> : std::false_type {};
718:template <typename T>
721:template <typename T>
729:template <typename T> struct allocator : private std::decay<void> {
747:template <typename Formatter>
752:template <typename Formatter>
776:template <typename T, size_t SIZE = inline_buffer_size,
828:  template <typename Alloc = Allocator,
836:  template <typename Alloc = Allocator,
894:  template <typename ContiguousRange>
902:template <size_t SIZE>
925:  template <typename... T> void print(format_string<T...> fmt, T&&... args) {
945:template <typename T, size_t SIZE, typename Allocator>
967:template <typename Char, size_t N> struct fixed_string {
976:FMT_EXPORT template <typename Char, size_t N>
983:FMT_EXPORT template <typename Char>
991:template <typename T, FMT_ENABLE_IF(is_signed<T>::value)>
995:template <typename T, FMT_ENABLE_IF(!is_signed<T>::value)>
1002:template <typename T>
1007:template <typename T>
1045:template <typename Char> constexpr auto getsign(sign s) -> Char {
1050:template <typename T> FMT_CONSTEXPR auto count_digits_fallback(T n) -> int {
1101:template <int BITS, typename UInt>
1150:template <typename Int> constexpr auto digits10() noexcept -> int {
1153:template <> constexpr auto digits10<native_int128>() noexcept -> int {
1156:template <> constexpr auto digits10<uint128_t>() noexcept -> int { return 38; }
1158:template <typename Char> struct thousands_sep_result {
1163:template <typename Char>
1165:template <typename Char>
1170:template <>
1175:template <typename Char>
1177:template <typename Char> inline auto decimal_point(locale_ref loc) -> Char {
1180:template <> inline auto decimal_point(locale_ref loc) -> wchar_t {
1186:extern template FMT_API auto thousands_sep_impl<char>(locale_ref)
1188:extern template FMT_API auto thousands_sep_impl<wchar_t>(locale_ref)
1190:extern template FMT_API auto decimal_point_impl(locale_ref) -> char;
1191:extern template FMT_API auto decimal_point_impl(locale_ref) -> wchar_t;
1196:template <typename Char> auto equal2(const Char* lhs, const char* rhs) -> bool {
1204:template <typename Char>
1215:template <typename Char>
1227:template <typename Char, typename UInt>
1256:template <typename Char, typename UInt>
1263:template <typename Char, typename UInt, typename OutputIt,
1278:template <typename Char, typename UInt>
1292:template <typename Char, typename UInt>
1299:template <typename Char, typename OutputIt, typename UInt,
1339:template <typename WChar, typename Buffer = memory_buffer> class to_utf8 {
1476:template <typename T, typename Enable = void> struct float_info;
1478:template <> struct float_info<float> {
1490:template <> struct float_info<double> {
1503:template <typename T>
1512:template <typename T>
1517:template <typename T> struct decimal_fp {
1523:template <typename T> FMT_API auto to_decimal(T x) noexcept -> decimal_fp<T>;
1527:template <typename Float> constexpr auto has_implicit_bit() -> bool {
1534:template <typename Float> constexpr auto num_significand_bits() -> int {
1541:template <typename Float>
1548:template <typename Float> constexpr auto exponent_bias() -> int {
1562:template <typename Char, typename OutputIt>
1589:template <typename F> struct basic_fp {
1600:  template <typename Float> FMT_CONSTEXPR basic_fp(Float n) { assign(n); }
1603:  template <typename Float, FMT_ENABLE_IF(!is_double_double<Float>::value)>
1628:  template <typename Float, FMT_ENABLE_IF(is_double_double<Float>::value)>
1638:template <int SHIFT = 0, typename F>
1677:template <typename T, bool doublish = num_bits<T>() == num_bits<double>()>
1681:template <typename T>
1686:template <bool C, typename T, typename F, FMT_ENABLE_IF(C)>
1690:template <bool C, typename T, typename F, FMT_ENABLE_IF(!C)>
1695:template <typename Char, typename OutputIt>
1709:template <typename Char, align default_align = align::left, typename OutputIt,
1730:template <typename Char, align default_align = align::left, typename OutputIt,
1737:template <typename Char, align default_align = align::left, typename OutputIt>
1747:template <typename Char, typename OutputIt, typename UIntPtr>
1770:template <typename Char> struct find_escape_result {
1776:template <typename Char>
1802:template <size_t width, typename Char, typename OutputIt>
1812:template <typename OutputIt, typename Char>
1849:template <typename Char, typename OutputIt>
1865:template <typename Char, typename OutputIt>
1881:template <typename Char, typename OutputIt>
1892:template <typename Char> class digit_grouping {
1933:  template <typename Out, typename C>
1962:template <typename OutputIt, typename UInt, typename Char>
2017:template <typename OutputIt>
2023:template <typename UInt> struct write_int_arg {
2028:template <typename T>
2043:template <typename Char = char> struct loc_writer {
2050:  template <typename T, FMT_ENABLE_IF(is_integer<T>::value)>
2058:  template <typename T, FMT_ENABLE_IF(!is_integer<T>::value)>
2064:// Size and padding computation separate from write_int to avoid template bloat.
2085:template <typename Char, typename OutputIt, typename T>
2149:template <typename Char, typename OutputIt, typename T>
2157:template <typename Char, typename T,
2170:template <typename Char, typename OutputIt, typename T,
2182:template <typename Char, typename OutputIt>
2193:template <typename Char, typename OutputIt,
2277:template <typename Char, typename OutputIt,
2300:template <typename Char, typename OutputIt>
2306:template <typename Char, typename OutputIt>
2315:template <typename Char, typename OutputIt, typename T,
2335:template <typename Char>
2370:template <typename Char, typename OutputIt>
2399:template <typename T>
2404:template <typename Char, typename OutputIt>
2409:template <typename Char, typename OutputIt, typename UInt>
2414:template <typename Char, typename OutputIt, typename T, typename Grouping>
2428:template <typename Char, typename UInt,
2450:template <typename OutputIt, typename UInt, typename Char,
2462:template <typename OutputIt, typename Char>
2474:template <typename OutputIt, typename Char, typename T, typename Grouping>
2494:template <typename T> FMT_CONSTEVAL auto exp_upper() -> int {
2506:template <typename Char> class fallback_digit_grouping {
2514:  template <typename Out, typename C>
2520:template <typename Char, typename Grouping, typename OutputIt,
2586:template <typename Char, typename Grouping, typename OutputIt,
2626:template <typename Char, typename OutputIt, typename DecimalFP>
2639:template <typename T> constexpr auto isnan(T value) -> bool {
2643:template <typename T, typename Enable = void>
2646:template <typename T>
2650:template <typename T,
2658:template <typename T, FMT_ENABLE_IF(!has_isfinite<T>::value)>
2665:template <typename T, FMT_ENABLE_IF(is_floating_point<T>::value)>
2738:  template <typename UInt, FMT_ENABLE_IF(std::is_same<UInt, uint64_t>::value ||
2759:  template <typename UInt, FMT_ENABLE_IF(std::is_same<UInt, uint64_t>::value ||
2786:  template <typename Int> FMT_CONSTEXPR void operator=(Int n) {
2810:  template <typename Int> FMT_CONSTEXPR auto operator*=(Int value) -> bigint& {
3071:template <typename Float, FMT_ENABLE_IF(!is_double_double<Float>::value)>
3151:template <typename Float, FMT_ENABLE_IF(is_double_double<Float>::value)>
3169:template <typename Float>
3468:template <typename Char, typename OutputIt, typename T,
3525:template <typename Char, typename OutputIt, typename T,
3591:template <typename Char, typename OutputIt, typename T,
3598:template <typename Char, typename OutputIt>
3605:template <typename Char, typename OutputIt>
3611:template <typename Char, typename OutputIt, typename T,
3618:template <
3627:template <typename Char, typename OutputIt, typename T,
3637:template <typename Char, typename OutputIt>
3644:template <typename Char, typename OutputIt>
3651:template <typename Char, typename OutputIt, typename T,
3658:template <typename Char, typename OutputIt, typename T,
3670:template <typename T>
3676:template <typename Char> struct default_arg_formatter {
3683:  template <typename T, FMT_ENABLE_IF(is_builtin<T>::value)>
3688:  template <typename T, FMT_ENABLE_IF(!is_builtin<T>::value)>
3701:template <typename Char> struct arg_formatter {
3706:  template <typename T, FMT_ENABLE_IF(is_builtin<T>::value)>
3711:  template <typename T, FMT_ENABLE_IF(!is_builtin<T>::value)>
3723:  template <typename T, FMT_ENABLE_IF(is_integer<T>::value)>
3728:  template <typename T, FMT_ENABLE_IF(!is_integer<T>::value)>
3735:template <typename Context>
3750:template <typename T, typename Char, size_t N,
3759:template <typename T, typename Char, size_t N,
3763:template <typename T, typename Char, size_t N,
3768:template <typename Char, size_t N, fmt::detail::fixed_string<Char, N> Str>
3770:  template <typename T> auto operator=(T&& value) const {
3775:template <typename Char> struct udl_arg {
3778:  template <typename T> auto operator=(T&& value) const -> named_arg<T, Char> {
3784:template <typename Char = char> struct format_handler {
3839:template <typename T, typename Char, type TYPE>
3840:template <typename FormatContext>
3859:template <typename OutputIt, typename Char> class generic_context {
3903:  template <typename T, FMT_ENABLE_IF(!detail::is_float128<T>::value)>
3906:  template <typename T, FMT_ENABLE_IF(detail::is_float128<T>::value)>
3909:  template <typename Visitor> auto visit(Visitor&& vis) -> decltype(vis(0)) {
3916:template <typename Locale> class format_facet : public Locale::facet {
3943:  template <typename Char>                                          \
3945:    template <typename FormatContext>                               \
3963:template <typename Char, size_t N>
3966:template <typename Char, typename Traits, typename Allocator>
3970:template <typename Char>
3975:template <typename T, typename Char>
3978:  template <typename FormatContext>
3993:template <typename T> auto ptr(T p) -> const void* {
4006:template <typename Enum>
4012:template <typename Enum, FMT_ENABLE_IF(std::is_enum<Enum>::value)>
4024:template <> struct formatter<bytes> {
4034:  template <typename FormatContext>
4046:template <typename T> struct group_digits_view {
4059:template <typename T> auto group_digits(T value) -> group_digits_view<T> {
4063:template <typename T> struct formatter<group_digits_view<T>> : formatter<T> {
4073:  template <typename FormatContext>
4088:template <typename T, typename Char> struct nested_view {
4093:template <typename T, typename Char>
4098:  template <typename FormatContext>
4105:template <typename T, typename Char = char> struct nested_formatter {
4130:  template <typename FormatContext, typename F>
4158:template <detail::fixed_string S> constexpr auto operator""_a() {
4186:  template <typename UInt>
4192:  template <typename Int>
4288:template <typename... T>
4321:template <typename... T>
4327:template <typename OutputIt,
4336:template <typename OutputIt, typename... T,
4343:template <typename... T>
4363:template <typename... T>
4376:template <typename T, FMT_ENABLE_IF(std::is_integral<T>::value)>
4384:template <typename T, FMT_ENABLE_IF(detail::use_format_as<T>::value)>
4390:template <typename T, FMT_ENABLE_IF(!std::is_integral<T>::value &&


<br>
