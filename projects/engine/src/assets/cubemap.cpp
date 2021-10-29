#include "cubemap.hpp"

Cubemap::Cubemap(Texture *top, Texture *bottom, Texture *right, Texture *left, Texture *forward, Texture *back) {
    this->top = top;
    this->bottom = bottom;
    this->right = right;
    this->left = left;
    this->forward = forward;
    this->back = back;

    this->buffer = nullptr;
}