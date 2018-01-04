template <class T> struct pod_allocator {
  static T *allocate(unsigned num) { return new T[num]; }
  static void deallocate(T *ptr, unsigned) { delete[] ptr; }
};

template <class T> struct obj_allocator {
  static T *allocate() { return new T; }
  static void deallocate(T *ptr) { delete ptr; }
};


template <class T>
void pod_vector<T>::capacity(unsigned cap, unsigned extra_tail) {
  m_size = 0;
  if (cap > m_capacity) {
    pod_allocator<T>::deallocate(m_array, m_capacity);
    m_capacity = cap + extra_tail;
    m_array = m_capacity ? pod_allocator<T>::allocate(m_capacity) : 0;
  }
}

template <class T, unsigned S = 6> class pod_bvector {};

template <class T, unsigned S>
pod_bvector<T, S>::pod_bvector(const pod_bvector<T, S> &v)
    : m_size(v.m_size), m_num_blocks(v.m_num_blocks),
      m_max_blocks(v.m_max_blocks),
      m_blocks(v.m_max_blocks ? pod_allocator<T *>::allocate(v.m_max_blocks)
                              : 0),
      m_block_ptr_inc(v.m_block_ptr_inc) {
  unsigned i;
  for (i = 0; i < v.m_num_blocks; ++i) {
    m_blocks[i] = pod_allocator<T>::allocate(block_size);
    memcpy(m_blocks[i], v.m_blocks[i], block_size * sizeof(T));
  }
}



class vcgen_stroke {
  enum status_e {
    initial,
    ready,
    cap1,
    cap2,
    outline1,
    close_first,
    outline2,
    out_vertices,
    end_poly1,
    end_poly2,
    stop
  };

public:
  typedef vertex_sequence<vertex_dist, 6> vertex_storage;
  typedef pod_bvector<point_d, 6> coord_storage;

  vcgen_stroke();

  void line_cap(line_cap_e lc) { m_stroker.line_cap(lc); }
  void line_join(line_join_e lj) { m_stroker.line_join(lj); }
  void inner_join(inner_join_e ij) { m_stroker.inner_join(ij); }

  line_cap_e line_cap() const { return m_stroker.line_cap(); }
  line_join_e line_join() const { return m_stroker.line_join(); }
  inner_join_e inner_join() const { return m_stroker.inner_join(); }

  void width(double w) { m_stroker.width(w); }
  void miter_limit(double ml) { m_stroker.miter_limit(ml); }
  void miter_limit_theta(double t) { m_stroker.miter_limit_theta(t); }
  void inner_miter_limit(double ml) { m_stroker.inner_miter_limit(ml); }
  void approximation_scale(double as) { m_stroker.approximation_scale(as); }

  double width() const { return m_stroker.width(); }
  double miter_limit() const { return m_stroker.miter_limit(); }
  double inner_miter_limit() const { return m_stroker.inner_miter_limit(); }
  double approximation_scale() const { return m_stroker.approximation_scale(); }

  void shorten(double s) { m_shorten = s; }
  double shorten() const { return m_shorten; }

  void remove_all();
  void add_vertex(double x, double y, unsigned cmd);

  void rewind(unsigned path_id);
  unsigned vertex(double *x, double *y);

private:
  vcgen_stroke(const vcgen_stroke &);
  const vcgen_stroke &operator=(const vcgen_stroke &);

  math_stroke<coord_storage> m_stroker;
  vertex_storage m_src_vertices;
  coord_storage m_out_vertices;
  double m_shorten;
  unsigned m_closed;
  status_e m_status;
  status_e m_prev_status;
  unsigned m_src_vertex;
  unsigned m_out_vertex;
};
