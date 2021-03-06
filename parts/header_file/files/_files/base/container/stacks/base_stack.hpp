//defs.hpp
//at namespace elc::defs::container::stack_n
/*
未完成的elc解释器base文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
template<typename T>
class base_stack_t:non_copyable{//基础容器，可用于hashtable
	typedef base_stack_t<T>this_t;
protected:
	struct data_t{
		T _data;
		data_t*_next;

		constexpr_as(construct<T>(declvalue(const T&)))data_t(const T&a,data_t*b)noexcept(construct<T>.nothrow<const T&>):_data(a),_next(b){}
		constexpr_as(construct<T>(declvalue(const T&)))data_t(const data_t&a)noexcept(construct<T>.nothrow<const T&>):_data(a._data),_next(null_ptr){}
	}*_m;
	size_t _size;
private:
	this_t copy()const noexcept(copy_get.nothrow<data_t>){
		this_t tmp;
		const data_t*p=_m;
		data_t**p_=&tmp._m;
		while(p!=null_ptr){
			*p_=copy_get(p);
			p_=&(**p_)._next;
			p=p->_next;
		}
		tmp._size = size();
		return tmp;
	}
public:
	constexpr base_stack_t()noexcept:_m(null_ptr),_size(0){}
	void swap_with(this_t&a)noexcept{
		swap(_m,a._m);
		swap(_size,a._size);
	}
	this_t&operator=(this_t&&a)&noexcept{
		swap_with(a);
		return*this;
	}
	base_stack_t(this_t&&a)noexcept:base_stack_t(){
		operator=(move(a));
	}
	this_t&operator=(const this_t&a)&noexcept_as(declvalue(this_t).copy()){
		return operator=(a.copy());
	}
	base_stack_t(const this_t&a)noexcept_as(declvalue(this_t).copy()):base_stack_t(a.copy()){}
	~base_stack_t()noexcept(unget.nothrow<data_t>){
		data_t*tmp;
		while(_m!=null_ptr){
			tmp=_m;
			_m=_m->_next;
			unget(tmp);
		}
	}
	void clear()noexcept(re_construct.nothrow<this_t>){
		re_construct(this);
	}
	[[nodiscard]]bool empty()const noexcept{
		return _m==null_ptr;
	}
	template<typename T_>
	[[nodiscard]]maybe_fail_reference<T>find(T_&&a)noexcept_as(declvalue(T&)==declvalue(T_)){
		data_t*tmp=_m;
		while(tmp!=null_ptr){
			if(tmp->_data==a)
				return tmp->_data;
			tmp=tmp->_next;
		}
		return note::fail;
	}
	[[nodiscard]]bool in_stack(const T&a)const noexcept_as(declvalue(const T&)==declvalue(T&)){
		return const_cast<this_t*>(this)->find(a).not_fail();
	}
	[[nodiscard]]bool not_in_stack(const T&a)const noexcept_as(declvalue(this_t).in_stack(declvalue(const T&))){
		return not in_stack(a);
	}
	[[nodiscard]]size_t size()const noexcept{
		return _size;
	}

	static constexpr bool add_nothrow=noexcept(get<data_t>(declvalue(const T&),declvalue(data_t*)));
	void add(const T&a)noexcept(add_nothrow){
		_m=get<data_t>(a,_m);
		_size++;
	}

	static constexpr bool remove_nothrow=unget.nothrow<data_t>;
	bool remove(const T&a)noexcept(remove_nothrow){//返回值：是否成功移除（容器里是否有此T）
		data_t*tmp=_m,**tmp_=&_m;
		while(tmp!=null_ptr){
			if(a==tmp->_data){
				_size--;
				data_t*remove_p=tmp;
				*tmp_=tmp->_next;
				unget(tmp);
				return 1;
			}
			tmp_=&tmp->_next;
			tmp=*tmp_;
		}
		return 0;
	}
	#define expr declvalue(func_t)(declvalue(T&))
	template<typename func_t,enable_if_not_ill_form(expr)>
	void for_each(func_t&&func)noexcept_as(expr){
		data_t*tmp=_m;
		while(tmp!=null_ptr){
			func(tmp->_data);
			tmp=tmp->_next;
		}
	}
	#undef expr

	#define expr declvalue(func_t)(declvalue(const T&))
	template<typename func_t,enable_if_not_ill_form(expr)>
	void for_each(func_t&&func)const noexcept_as(expr){
		const data_t*tmp=_m;
		while(tmp!=null_ptr){
			func(tmp->_data);
			tmp=tmp->_next;
		}
	}
	#undef expr
protected:
	void add(data_t*a){
		a->_next=_m;
		_m=a;
		_size++;
	}
public:
	[[nodiscard]]hash_t get_top_hash()noexcept_as(hash(declvalue(T&))){
		if(_m)
			return hash(_m->_data);
	}
	void move_top_to(this_t&a)noexcept{
		size--;
		auto tmp=_m;
		_m=_m->_next;
		a.add(tmp);
	}
};

//file_end

