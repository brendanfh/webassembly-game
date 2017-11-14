#ifndef __RECT_H_
#define __RECT_H_

class Rect {
public:
    float x;
    float y;
    float w;
    float h;

    Rect() {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { }

    ~Rect() { }

    void Set(float x, float y, float w, float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    bool Intersects(Rect r2) {
        return (   r2.x + r2.w >= x
                && x + w >= r2.x
                && r2.y + r2.h >= y
                && y + h >= r2.y);
    }
    
    bool Contains(Rect r2) {
        return (r2.x > x && r2.x + r2.w < x + w && r2.y > y && r2.y + r2.h < y + h);
    }
};

#endif 