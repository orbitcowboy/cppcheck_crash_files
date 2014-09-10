template <class T>
class PoolT
{
	struct PoolEntry { };

public:
	typedef std::vector<PoolEntry>	PoolVector;

private:
	static PoolVector	m_pool_vector;
	static size_t		m_pool_new_index;
	static size_t		m_pool_delete_index;
	static Lock			m_pool_lock;
};

class Fred
{
public:
 	struct Msg1 : public PoolT<Msg1> { };
	struct Msg2 : public PoolT<Msg2> { };
	struct Msg3 : public PoolT<Msg3> { };
	struct Msg4 : public PoolT<Msg4> { };
	struct Msg5 : public PoolT<Msg5> { };
	struct Msg6 : public PoolT<Msg6> { };
	struct Msg7 : public PoolT<Msg7> { };
	struct Msg8 : public PoolT<Msg8> { };
	struct Msg9 : public PoolT<Msg9> { };
};

template<> Fred::Msg1::PoolVector			PoolT<Fred::Msg1>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg1>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg1>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg1>::m_pool_lock(0);

template<> Fred::Msg2::PoolVector			PoolT<Fred::Msg2>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg2>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg2>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg2>::m_pool_lock(0);

template<> Fred::Msg3::PoolVector			PoolT<Fred::Msg3>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg3>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg3>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg3>::m_pool_lock(0);

template<> Fred::Msg4::PoolVector			PoolT<Fred::Msg4>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg4>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg4>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg4>::m_pool_lock(0);

template<> Fred::Msg5::PoolVector			PoolT<Fred::Msg5>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg5>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg5>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg5>::m_pool_lock(0);

template<> Fred::Msg6::PoolVector			PoolT<Fred::Msg6>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg6>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg6>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg6>::m_pool_lock(0);

template<> Fred::Msg7::PoolVector			PoolT<Fred::Msg7>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg7>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg7>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg7>::m_pool_lock(0);

template<> Fred::Msg8::PoolVector			PoolT<Fred::Msg8>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg8>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg8>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg8>::m_pool_lock(0);

template<> Fred::Msg9::PoolVector			PoolT<Fred::Msg9>::m_pool_vector(0);
template<> size_t							PoolT<Fred::Msg9>::m_pool_new_index = 0;
template<> size_t							PoolT<Fred::Msg9>::m_pool_delete_index = 0;
template<> Lock								PoolT<Fred::Msg9>::m_pool_lock(0);
