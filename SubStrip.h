#ifndef __SUBSTRIP__H
#define __SUBSTRIP__H
#include <FastLED.h>
#include <vector>

class SubStrip
{
public:
    CRGB *leds;
    SubStrip() {}

    SubStrip(CRGB *start, int length)
    {
        this->leds = start;
        this->_length = length;
    }

    virtual int numPixels()
    {
        return _length;
    }

    virtual void fill(const struct CRGB &color)
    {
        fill_solid(leds, _length, color);
    }
    virtual void fill(const struct CRGB &color, int start, int length)
    {
        if(start + length > _length)
            length = _length - start; // no crashy plssss
        fill_solid(leds+start, length, color);
    }

    virtual struct CRGB& operator[](int x)
    {
        if(x >= _length)
            x = 0; // avoid crash :P
        return leds[x];
    }
    virtual void set(int x, CRGB v)
    {
        if(x >= _length) return;

        leds[x] = v;
    }
private:
    int _length;
};

class ProxyStrip : public SubStrip
{
public:
    std::vector<SubStrip*> strips;

    ProxyStrip(std::vector<SubStrip*> strips)
      : strips(strips)
    {
    }

    int numPixels()
    {
        return strips[0]->numPixels();
    }

    void fill(const struct CRGB &color)
    {
        for(const auto& strip: strips)
            strip->fill(color);
    }
    void fill(const struct CRGB &color, int start, int length)
    {
        for(const auto strip: strips)
            strip->fill(color, start, length);
    }

    struct CRGB& operator[](int x)
    {
        return (*strips[0])[x];
    }
    virtual void set(int x, CRGB v)
    {
        for(const auto strip: strips)
            strip->set(x, v);
    }
};

template <typename T> T clamp(T value, T low, T high)
{
    return (value < low) ? low : ((value > high) ? high : value);
}

extern const uint8_t g_gamma8[256];

uint8_t gamma8(uint8_t x)
{
    return g_gamma8[x];
}

float gammaf(float f)
{
    // stupid, I can't find lgammaf in math.h on arduino?
    return gamma8(f*255)/(float)255;
}

CRGB operator*( const CRGB& p1, float f)
{
    return CRGB(p1.r*f, p1.g*f, p1.b*f);
}

#endif
