//deduction_guides.hpp
//at namespace elc::defs::memory::ptr_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<class T>
root_ptr_t(T*) -> root_ptr_t<T>;

template<class T>
root_ptr_t(convert_interface_t<T>) -> root_ptr_t<T>;

//file_end
