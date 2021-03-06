//base_function_t.hpp
//at namespace elc::defs::container
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace function_n{
	template<class T>
	class base_func_data_t;
	template<class Ret_t,class...Args_t>
	struct base_func_data_t<Ret_t(Args_t...)>:ref_able<base_func_data_t<Ret_t(Args_t...)>>,build_by_get_only,never_in_array<base_func_data_t<Ret_t(Args_t...)>>{
		typedef base_func_data_t<Ret_t(Args_t...)>this_t;

		virtual ~base_func_data_t()=default;
		virtual Ret_t call(Args_t...)=0;
		//for equal:
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept=0;
		[[nodiscard]]virtual const void*get_data_begin()const noexcept=0;
		[[nodiscard]]virtual bool equal_with(const void*)const=0;
		[[nodiscard]]bool operator==(const this_t&a)const{
			return this->get_type_info()==a.get_type_info()&&this->equal_with(a.get_data_begin());
		}
	};

	template<class T,class Func_t>
	class func_data_t;
	template<class T,class Ret_t,class...Args_t>
	struct func_data_t<T,Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>{
		static_assert(is_not_function(T));
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		T _value;

		func_data_t(T&a)noexcept(construct<T>.nothrow<T>):_value(a){}
		virtual ~func_data_t()noexcept(destruct.nothrow<T>)override=default;
		virtual Ret_t call(Args_t...args)override{
			//BLOCK:constexpr checks
			if constexpr(!invoke<T>.able<Args_t...>)
				template_error("this T can\'t becall as args.");
			if constexpr(type_info<decltype(declvalue(T)(declvalue(Args_t)...))> != type_info<Ret_t>)
				template_error("the return type of T was wrong.");
			//BLOCK_END
			return _value(forward<Args_t>(args)...);
		}
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<T>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return reinterpret_cast<const void*>(&_value);}
		[[nodiscard]]virtual bool equal_with(const void*a)const override{
			if constexpr(equality_comparable<T>)
				return _value==*reinterpret_cast<const T*>(a);
			else
				return false;
		}
	};

	template<class T>
	class default_func_data_t;
	template<class Ret_t,class...Args_t>
	struct default_func_data_t<Ret_t(Args_t...)>:base_func_data_t<Ret_t(Args_t...)>{
		typedef base_func_data_t<Ret_t(Args_t...)>base_t;

		virtual ~default_func_data_t()noexcept override{}
		virtual Ret_t call(Args_t...)override{return Ret_t();}
		[[nodiscard]]virtual const base_type_info_t&get_type_info()const noexcept override{return type_info<void>;}
		[[nodiscard]]virtual const void*get_data_begin()const noexcept override{return null_ptr;}
		[[nodiscard]]virtual bool equal_with(const void*a)const noexcept override{return true;}
	};
	template<class Ret_t,class...Args_t>
	inline constexpr default_func_data_t<Ret_t(Args_t...)>default_func_data{};

	template<class Ret_t,class...Args_t>
	[[nodiscard]]constexpr base_func_data_t<Ret_t(Args_t...)>*get_null_ptr(type_info_t<base_func_data_t<Ret_t(Args_t...)>>)noexcept{
		return&default_func_data<Ret_t,Args_t...>;
	}

	template<class T>
	class function_data_saver_t;
	template<class Ret_t,class...Args_t>
	class function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>this_t;
		typedef comn_ptr_t<base_func_data_t<Ret_t(Args_t...)>>ptr_t;
		ptr_t _m;
		void swap_with(this_t&a)noexcept{swap(_m,a._m);}
	public:
		function_data_saver_t()noexcept=default;
		function_data_saver_t(const this_t&a)noexcept:_m(a._m){}
		function_data_saver_t(this_t&&a)noexcept{swap_with(a);}
		[[nodiscard]]bool operator==(const this_t&a)const noexcept{
			return *_m==*(a._m);
		}
		void operator=(const this_t&a){_m=a._m;}
		Ret_t call(Args_t...)const{return _m->call(forward<Args_t>(args)...);}
	};

	template<class T,bool nothrow,bool promise_nothrow_at_destruct>
	class base_function_t;
	template<class Ret_t,class...Args_t,bool nothrow,bool promise_nothrow_at_destruct>
	struct base_function_t<Ret_t(Args_t...),nothrow,promise_nothrow_at_destruct>:function_data_saver_t<Ret_t(Args_t...)>{
	protected:
		typedef function_data_saver_t<Ret_t(Args_t...)>base_t;
		typedef base_function_t<Ret_t(Args_t...),nothrow,promise_nothrow_at_destruct>this_t;

		template<class,bool,bool>
		friend class base_function_t;

		template<class T_>
		using func_data_t=function_n::func_data_t<T_,Ret_t(Args_t...)>;

		typedef Ret_t(*func_ptr_t)(Args_t...)noexcept(nothrow);

		using base_t::ptr_t;
		using base_t::_m;

		template<class T>
		static constexpr bool base_on_this_t_or_more_stringent_restrictions=(
			type_info<T>.base_on<this_t>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...),bool(nothrow+1),promise_nothrow_at_destruct>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...),nothrow,bool(promise_nothrow_at_destruct+1)>>||
			type_info<T>.base_on<base_function_t<Ret_t(Args_t...),bool(nothrow+1),bool(promise_nothrow_at_destruct+1)>>
		);
	public:
		void swap_with(this_t&a)noexcept{//不与base_t::swap_with重复：与更加严格（或宽松）的this_t进行swap是有风险的
			base_t::swap_with(a);
		}

		base_function_t()noexcept=default;
		template<class T,enable_if(base_on_this_t_or_more_stringent_restrictions<T>)>
		base_function_t(const T&a)noexcept:base_t(a){}
		base_function_t(const this_t&a)noexcept:base_t(a){}
		base_function_t(this_t&&a)noexcept:base_function_t(){
			swap_with(a);
		}
		base_function_t(nullptr_t)noexcept:base_function_t(){}
		base_function_t(null_ptr_t)noexcept:base_function_t(){}
		template<class T,enable_if(invoke<T>.able<Args_t...>&&not base_on_this_t_or_more_stringent_restrictions<T>)>
		base_function_t(T&&a)noexcept(get<func_data_t<remove_cvref<T>>>.nothrow<T>){
			//BLOCK:constexpr checks
			if constexpr(promise_nothrow_at_destruct and not destruct.nothrow<T>)
				template_error("unexpected assign.");
			if constexpr(nothrow)
				if constexpr(!invoke<T>.nothrow<Args_t...>)
					template_warning("the call of T was not noexcept,this may cause terminate.");
			//BLOCK_END
			_m=get<func_data_t<remove_cvref<T>>>(a);
		}
		base_function_t(func_ptr_t a)noexcept{//当nothrow==0时，noexcept(1)的参数可自动转为noexcept(0)的，不用再次考虑
			_m=get<func_data_t<func_ptr_t>>(a);
		}
		~base_function_t()noexcept(promise_nothrow_at_destruct)=default;

		template<class T,enable_if_not_ill_form(declvalue(this_t).swap_with(this_t(declvalue(T))))>
		this_t&operator=(T&&a)&noexcept_as(declvalue(this_t).swap_with(this_t(declvalue(T)))){
			this_t tmp(forward<T>(a));
			swap_with(tmp);
			return*this;
		}

		template<class T,enable_if(base_on_this_t_or_more_stringent_restrictions<T>)>
		this_t&operator=(const T&a)&noexcept(promise_nothrow_at_destruct){
			base_t::operator=(a);
			return*this;
		}

		Ret_t operator()(Args_t...args)const noexcept(nothrow){
			return base_t::call(forward<Args_t>(args)...);
		}
		/*
		private:
			//以下是突然想加的功能(没什么用<迷惑行为大赏>).
			static ptr_t _func_ptr_data;
			static Ret_t _func_ptr_value(Args_t...args)noexcept(nothrow){
				return _func_ptr_data->call(forward<Args_t>(args)...);
			}
		public:
			[[nodiscard]]explicit operator func_ptr_t()const noexcept(promise_nothrow_at_destruct){
				_func_ptr_data=base_t::_m;
				return _func_ptr_value;
			}
		*/
	};
}

//file_end

