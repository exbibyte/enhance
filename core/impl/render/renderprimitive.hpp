
namespace e2 { namespace render {

class renderprim {
public:
    float _color[3];
    float _bbox_low[3];
    float _bbox_high[3];
};

class renderprim_dot : public renderprim {
public:
    float _radius;
    float _coord[3];
};

class renderprim_rect : public renderprim {
public:
    float _coord_low[3];
    float _coord_high[3];
};

class renderprim_sphere : public renderprim {
public:
    float _radius;
    float _coord[3];
};

class renderprim_line : public renderprim {
public:
    float _length;
    float _coord_low[3];
    float _coord_high[3];
};

} }
