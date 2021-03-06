//_defs.hpp
/*
未完成的elc解释器_share文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4099)//class与struct混用警告diss
	#pragma warning(disable:26812)//enum class安利diss.
	#pragma warning(disable:4584)//重复子类警告diss
#endif
//

#define lazy_instantiation template<int ILD_value>
#define lazy_instantiation_name(name) typedef LIS_name(name)<LIS_default_value> name
#define LIS_ID_t(name) LIS_name(name)LIS_IS_flag
#define LIS_name(name) name ## _LIS_t
#define LIS_IS_flag <ILD_value>
#define LIS_default_value magic_number::god

// #define often_noexcept
#define noexcept_as(...) noexcept(noexcept(__VA_ARGS__))//dnmd C艹标准就是没有noexcept_as_auto
#define noexcept_as_auto MAGIC//哦现在有了
#define constexpr_as(...) MAGIC constexpr//( •̀ ω •́ )✌
#define constexpr_as_auto MAGIC MAGIC constexpr//✌( •̀ ω •́ )✌
#define is_not_function(...) (not is_function(__VA_ARGS__))
#define is_function(...) ::std::is_function_v<__VA_ARGS__>

#define using_method_from_base_t(name) \
template<class...Args,enable_if_not_ill_form(declvalue(base_t).name(declvalue(Args)...))>\
auto name(Args&&...rest)noexcept_as(base_t::name(declvalue(Args)...))\
{\
	return base_t::name(forward<Args>(rest)...);\
}\

#define using_method_from_value(name,value_name) \
template<class...Args,enable_if_not_ill_form(re_declvalue(value_name).name(declvalue(Args)...))>\
auto name(Args&&...rest)noexcept_as(re_declvalue(value_name).name(declvalue(Args)...))\
{\
	return value_name.name(forward<Args>(rest)...);\
}\


// #define floop while(1)
#define re_declvalue(...) (declvalue(decltype(__VA_ARGS__)))
#define re_decltype(...) (decltype(declvalue(__VA_ARGS__)))

#define declvalue(...) (::std::declval<__VA_ARGS__>())

#define template_error(reason) static_assert(template_error_helper<T>,reason)
#define template_warning(reason) template_warning_helper<T>(reason)

#define has_attribute(...) has_attribute_helper<__VA_ARGS__>()
#define not_has_attribute(...) not_has_has_attribute_helper<__VA_ARGS__>()

#define def_common_attribute_with_nothing(name) \
template<typename T>\
class name{}\

#define def_special_attribute_with_nothing(name) \
class name{}\

#define common_attribute_t template<class>class

/*
为什么不用cpp20的constraints？
1.不能类内定义
2.不能作为模板参数
一句话总结：c艹标准会sb
*/
#define enable_if(...) class enable_state= ::std::enable_if_t<__VA_ARGS__>
#define enabled_by_default class enable_state=void
#define disabled_by_default class enable_state= ::std::enable_if_t<false>
#define enable_if_not_ill_form(...) class enable_state= ::std::void_t<decltype(__VA_ARGS__)>
#define enable_flag class enable_state

#define was_an_ill_form(...) MAGIC//这个表达式应当返回一个bool，现在它做到了

#define lambda []
#define lambda_with_catch(...) [__VA_ARGS__]

#define MAGIC//ahh,ko no tenno da!

#define nothing//装饰性语法糖

#define elseif else if
#define _big_than_  >
#define _small_than_  <

#if defined(_MSC_VER)
	#define not !
	#define and &&
	#define or ||
#endif

#define INTER_NAMESPACE(NAMESPACENAME) namespace NAMESPACENAME{
#define BREAK_NAMESPACE }

//file_end

