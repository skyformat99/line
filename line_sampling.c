#include "svpng.inc"
#include <math.h> // fminf(), fmaxf(), sinf(), cosf()
#define PI 3.14159265359f

static const int w = 512, h = 512;
static unsigned char img[w * h * 3];

int capsule(float px, float py, float ax, float ay, float bx, float by, float r) {
    float pax = px - ax, pay = py - ay, bax = bx - ax, bay = by - ay;
    float h = fmaxf(fminf((pax * bax + pay * bay) / (bax * bax + bay * bay), 1.0f), 0.0f);
    float dx = pax - bax * h, dy = pay - bay * h;
    return dx * dx + dy * dy < r * r;
}

float sample(float x, float y) {
    float s = 0.0f, cx = w * 0.5f, cy = h * 0.5f;
    for (int j = 0; j < 5; j++) {
        float r1 = fmaxf(w, h) * (j + 0.5f) * 0.085f;
        float r2 = fmaxf(w, h) * (j + 1.5f) * 0.085f;
        float t = j * PI / 64.0f, r = (j + 1) * 0.5f;
        for (int i = 1; i <= 64; i++, t += 2.0f * PI / 64.0f) {
            float ct = cosf(t), st = sinf(t);
            s = fmaxf(s, capsule(x, y, cx + r1 * ct, cy - r1 * st, cx + r2 * ct, cy - r2 * st, r) ? 1.0f : 0.0f);
        }
    }
    return s;
}

int main() {
    unsigned char *p = img;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++, p += 3)
            p[0] = p[1] = p[2] = (unsigned char)((1.0f - sample(x, y)) * 255);
    svpng(fopen("line_sampling.png", "w"), 512, 512, img, 0);
}
