#pragma once
#include "epd_driver.h"
const uint8_t Roboto2BBitmaps[1280] = {
    0x78, 0x9C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x78, 0x9C, 0x63, 0xE7, 0x60, 0x65, 0x00, 0x00,
    0x00, 0x42, 0x00, 0x15, 0x78, 0x9C, 0xD3, 0x64, 0x00, 0x00, 0x00, 0x54, 0x00, 0x2A, 0x78, 0x9C,
    0x2B, 0x62, 0x58, 0xC1, 0x58, 0xCE, 0x00, 0x00, 0x06, 0x43, 0x01, 0x93, 0x78, 0x9C, 0x13, 0x60,
    0x98, 0xC6, 0x50, 0xCA, 0xB0, 0x9C, 0x41, 0x81, 0x01, 0x00, 0x0A, 0xF4, 0x01, 0xE3, 0x78, 0x9C,
    0x0B, 0x63, 0x6C, 0x66, 0x2A, 0x62, 0x67, 0x60, 0x00, 0x00, 0x07, 0xB8, 0x01, 0x56, 0x78, 0x9C,
    0x2B, 0x65, 0x98, 0xC1, 0x74, 0x92, 0x85, 0x81, 0x01, 0x00, 0x0A, 0x7A, 0x01, 0xDD, 0x78, 0x9C,
    0x63, 0x63, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x07, 0x78, 0x9C, 0x63, 0x10, 0x65, 0x67, 0x57, 0x06,
    0x00, 0x00, 0x9F, 0x00, 0x47, 0x78, 0x9C, 0x63, 0x60, 0x57, 0x15, 0x63, 0x05, 0x00, 0x00, 0xC1,
    0x00, 0x48, 0x78, 0x9C, 0x73, 0x73, 0x05, 0x00, 0x00, 0xD3, 0x00, 0x8C, 0x78, 0x9C, 0x53, 0x64,
    0xD8, 0xC1, 0xE0, 0xC4, 0x00, 0x00, 0x04, 0x30, 0x01, 0x1C, 0x78, 0x9C, 0x63, 0x66, 0x01, 0x00,
    0x00, 0x0C, 0x00, 0x08, 0x78, 0x9C, 0x53, 0x65, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x26, 0x78, 0x9C,
    0x63, 0x61, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x05, 0x78, 0x9C, 0x13, 0x60, 0x4E, 0x60, 0x48, 0x60,
    0x10, 0x60, 0x00, 0x00, 0x04, 0x7D, 0x00, 0xE4, 0x78, 0x9C, 0x9B, 0xCE, 0x50, 0xC1, 0xB8, 0x82,
    0x81, 0x81, 0x01, 0x00, 0x0A, 0x35, 0x01, 0xB9, 0x78, 0x9C, 0x4B, 0x9D, 0x30, 0x01, 0x00, 0x02,
    0xE2, 0x01, 0x86, 0x78, 0x9C, 0x6B, 0x67, 0x98, 0xC0, 0xD0, 0xC5, 0x08, 0x00, 0x06, 0x85, 0x01,
    0xA3, 0x78, 0x9C, 0x9B, 0xC6, 0xB0, 0x91, 0x61, 0x39, 0x03, 0x03, 0x03, 0x00, 0x0B, 0x7A, 0x01,
    0xEF, 0x78, 0x9C, 0x5B, 0xC0, 0xB0, 0x9C, 0x61, 0x2B, 0x23, 0x00, 0x07, 0xCD, 0x01, 0xFE, 0x78,
    0x9C, 0x2B, 0x67, 0x98, 0xC6, 0xB0, 0x8C, 0x81, 0x81, 0x01, 0x00, 0x09, 0xDC, 0x01, 0xB4, 0x78,
    0x9C, 0x0B, 0x61, 0x98, 0xC5, 0xB0, 0x82, 0x81, 0x81, 0x01, 0x00, 0x08, 0xE4, 0x01, 0x97, 0x78,
    0x9C, 0x5B, 0xC6, 0xD0, 0xC0, 0x60, 0xCA, 0x00, 0x00, 0x06, 0x54, 0x01, 0x5C, 0x78, 0x9C, 0x6B,
    0x63, 0xD8, 0xC1, 0xB0, 0x82, 0x81, 0x81, 0x01, 0x00, 0x0B, 0x28, 0x01, 0xE7, 0x78, 0x9C, 0x6B,
    0x67, 0xD8, 0xC1, 0x38, 0x99, 0x81, 0x81, 0x01, 0x00, 0x0A, 0xE1, 0x01, 0xD4, 0x78, 0x9C, 0x63,
    0x60, 0x66, 0x61, 0x00, 0x00, 0x00, 0x15, 0x00, 0x08, 0x78, 0x9C, 0x63, 0x60, 0x66, 0x66, 0x01,
    0x00, 0x00, 0x17, 0x00, 0x0B, 0x78, 0x9C, 0x63, 0xC8, 0x48, 0x02, 0x00, 0x01, 0x35, 0x00, 0xCB,
    0x78, 0x9C, 0x6B, 0x63, 0x48, 0x61, 0x00, 0x00, 0x02, 0xE4, 0x00, 0xEB, 0x78, 0x9C, 0x63, 0x68,
    0x33, 0x03, 0x00, 0x01, 0x45, 0x00, 0xBD, 0x78, 0x9C, 0x2B, 0x2F, 0x12, 0x66, 0x00, 0x00, 0x03,
    0x5C, 0x00, 0xFD, 0x78, 0x9C, 0x0B, 0x64, 0x4D, 0x0B, 0x6B, 0xB5, 0x0C, 0x61, 0x02, 0x00, 0x0A,
    0x2E, 0x02, 0x27, 0x78, 0x9C, 0x5B, 0xC8, 0x30, 0x8D, 0x31, 0x8B, 0x1D, 0x00, 0x07, 0x02, 0x01,
    0xAA, 0x78, 0x9C, 0xEB, 0x64, 0x5A, 0xC5, 0x34, 0x8B, 0x19, 0x00, 0x07, 0x2B, 0x01, 0xD5, 0x78,
    0x9C, 0x9B, 0xC6, 0xC4, 0xC9, 0x30, 0x83, 0x85, 0x81, 0x01, 0x00, 0x07, 0x68, 0x01, 0x3E, 0x78,
    0x9C, 0xEB, 0x64, 0xB4, 0x64, 0x9D, 0xC5, 0x04, 0x00, 0x05, 0x6A, 0x01, 0x65, 0x78, 0x9C, 0xAB,
    0x64, 0xAC, 0x62, 0xA8, 0x62, 0x04, 0x00, 0x05, 0xBE, 0x01, 0x70, 0x78, 0x9C, 0xAB, 0x64, 0xA8,
    0x62, 0xE0, 0x64, 0x00, 0x00, 0x04, 0xD6, 0x00, 0xFD, 0x78, 0x9C, 0x6B, 0x63, 0xF6, 0x64, 0x69,
    0x67, 0x63, 0x60, 0x00, 0x00, 0x08, 0x45, 0x01, 0x64, 0x78, 0x9C, 0xE3, 0x60, 0xAF, 0xE2, 0xE4,
    0xE4, 0x04, 0x00, 0x02, 0x77, 0x00, 0xA5, 0x78, 0x9C, 0x63, 0xE7, 0xE4, 0x04, 0x00, 0x00, 0x33,
    0x00, 0x1A, 0x78, 0x9C, 0x2B, 0x98, 0xB0, 0x9C, 0x01, 0x00, 0x04, 0xC2, 0x01, 0xA8, 0x78, 0x9C,
    0xCB, 0x60, 0x5A, 0xCD, 0x50, 0xC9, 0x02, 0x00, 0x06, 0x22, 0x01, 0x93, 0x78, 0x9C, 0xE3, 0x60,
    0xE0, 0x64, 0xA8, 0x62, 0x00, 0x00, 0x01, 0x4E, 0x00, 0x8C, 0x78, 0x9C, 0xD3, 0xB2, 0xA8, 0xF4,
    0x99, 0xE9, 0x0E, 0x00, 0x06, 0x5B, 0x02, 0x08, 0x78, 0x9C, 0x93, 0x64, 0xEF, 0xE2, 0x8C, 0xE4,
    0x04, 0x00, 0x03, 0xBD, 0x01, 0x16, 0x78, 0x9C, 0x9B, 0xC6, 0xCC, 0xC9, 0x31, 0x9D, 0x85, 0x81,
    0x01, 0x00, 0x07, 0x93, 0x01, 0x46, 0x78, 0x9C, 0xEB, 0x64, 0xEA, 0x62, 0xE1, 0x64, 0x00, 0x00,
    0x05, 0x8C, 0x01, 0x23, 0x78, 0x9C, 0x9B, 0xC6, 0xCC, 0xC9, 0x31, 0x9D, 0x55, 0x80, 0x19, 0x00,
    0x07, 0xB9, 0x01, 0x5A, 0x78, 0x9C, 0xEB, 0x64, 0x5A, 0xC5, 0x94, 0xC9, 0x0C, 0x00, 0x06, 0xC9,
    0x01, 0xA4, 0x78, 0x9C, 0x6B, 0x67, 0x6C, 0x65, 0x98, 0xCE, 0xCC, 0xC0, 0x00, 0x00, 0x09, 0xCA,
    0x01, 0xA8, 0x78, 0x9C, 0x5B, 0xCE, 0xD4, 0xC8, 0xD0, 0xC8, 0x00, 0x00, 0x07, 0x00, 0x01, 0xAC,
    0x78, 0x9C, 0x53, 0x67, 0xB5, 0x64, 0x9B, 0xC1, 0xCC, 0xC0, 0x00, 0x00, 0x05, 0x40, 0x01, 0x07,
    0x78, 0x9C, 0xD3, 0x60, 0xEB, 0x60, 0x3A, 0xC4, 0x00, 0x00, 0x04, 0xBE, 0x01, 0x7B, 0x78, 0x9C,
    0x6B, 0x77, 0x99, 0xA1, 0x9D, 0xC6, 0x0B, 0x00, 0x08, 0x3E, 0x02, 0x02, 0x78, 0x9C, 0xCB, 0x60,
    0x3E, 0xC2, 0xD0, 0xC9, 0x0C, 0x00, 0x06, 0xAA, 0x01, 0xBC, 0x78, 0x9C, 0x8B, 0x60, 0x5E, 0xC2,
    0xD0, 0xC8, 0x00, 0x00, 0x05, 0xB7, 0x01, 0x81, 0x78, 0x9C, 0x5B, 0xC6, 0xD4, 0xC8, 0xD0, 0xCD,
    0x04, 0x00, 0x07, 0x10, 0x01, 0xB7, 0x78, 0x9C, 0x63, 0xE6, 0xE4, 0xE4, 0x64, 0x07, 0x00, 0x00,
    0x6C, 0x00, 0x26, 0x78, 0x9C, 0x63, 0x17, 0x2F, 0x54, 0x00, 0x00, 0x01, 0x67, 0x00, 0xB0, 0x78,
    0x9C, 0x63, 0xE1, 0xE4, 0xE4, 0x64, 0x07, 0x00, 0x00, 0x71, 0x00, 0x27, 0x78, 0x9C, 0x33, 0x73,
    0x06, 0x00, 0x00, 0xB1, 0x00, 0x7A, 0x78, 0x9C, 0x0B, 0x07, 0x00, 0x00, 0x58, 0x00, 0x58, 0x78,
    0x9C, 0x63, 0x05, 0x00, 0x00, 0x06, 0x00, 0x06, 0x78, 0x9C, 0x63, 0x60, 0xD8, 0xCE, 0xB0, 0x93,
    0x81, 0x81, 0x01, 0x00, 0x07, 0x36, 0x01, 0x71, 0x78, 0x9C, 0x93, 0x64, 0x58, 0x05, 0x84, 0x0C,
    0x0C, 0x00, 0x07, 0x74, 0x01, 0x6E, 0x78, 0x9C, 0x63, 0xC8, 0xAC, 0x64, 0x00, 0x00, 0x02, 0x31,
    0x00, 0xE3, 0x78, 0x9C, 0x6B, 0x60, 0xD8, 0x09, 0x84, 0x0C, 0x0C, 0x00, 0x0B, 0x42, 0x01, 0xF3,
    0x78, 0x9C, 0x13, 0x60, 0xD8, 0xC5, 0x30, 0x93, 0x81, 0x81, 0x01, 0x00, 0x07, 0x48, 0x01, 0x64,
    0x78, 0x9C, 0x63, 0xB0, 0x90, 0xE2, 0x04, 0x00, 0x00, 0xE9, 0x00, 0x5C, 0x78, 0x9C, 0x13, 0x60,
    0xD8, 0xC9, 0xB8, 0x83, 0xA1, 0x85, 0x01, 0x00, 0x08, 0xCB, 0x02, 0x07, 0x78, 0x9C, 0x93, 0x64,
    0x58, 0xC5, 0xD0, 0xC9, 0x00, 0x00, 0x04, 0x56, 0x01, 0x4D, 0x78, 0x9C, 0x63, 0xE5, 0xE4, 0x04,
    0x00, 0x00, 0x2D, 0x00, 0x18, 0x78, 0x9C, 0x0B, 0x98, 0x30, 0xA1, 0x10, 0x00, 0x04, 0x85, 0x01,
    0xE2, 0x78, 0x9C, 0x93, 0x64, 0xE8, 0x62, 0x98, 0xC9, 0x00, 0x00, 0x03, 0xF6, 0x01, 0x3D, 0x78,
    0x9C, 0xE3, 0xE4, 0xE4, 0x04, 0x00, 0x00, 0x39, 0x00, 0x1C, 0x78, 0x9C, 0x13, 0x60, 0xDC, 0xA5,
    0xD9, 0xE9, 0x0A, 0x00, 0x05, 0x25, 0x01, 0xC3, 0x78, 0x9C, 0x13, 0x60, 0x58, 0xC5, 0xD0, 0xC9,
    0x00, 0x00, 0x04, 0x20, 0x01, 0x44, 0x78, 0x9C, 0x13, 0x60, 0x98, 0xC9, 0xB0, 0x92, 0x81, 0x81,
    0x01, 0x00, 0x06, 0xC2, 0x01, 0x53, 0x78, 0x9C, 0x13, 0x60, 0x58, 0x05, 0x84, 0xEC, 0x0C, 0x00,
    0x07, 0x3A, 0x01, 0x6C, 0x78, 0x9C, 0x13, 0x60, 0xD8, 0x09, 0x84, 0x09, 0x0C, 0x00, 0x08, 0x82,
    0x01, 0xE3, 0x78, 0x9C, 0x63, 0xD4, 0xE2, 0x04, 0x00, 0x00, 0x63, 0x00, 0x35, 0x78, 0x9C, 0x63,
    0xA8, 0x5C, 0xC6, 0x00, 0x00, 0x02, 0xBB, 0x01, 0x20, 0x78, 0x9C, 0x63, 0x93, 0x96, 0x60, 0x00,
    0x00, 0x00, 0x9D, 0x00, 0x3A, 0x78, 0x9C, 0x63, 0x60, 0xE8, 0x64, 0xD8, 0xC9, 0xC0, 0xC0, 0x00,
    0x00, 0x06, 0x22, 0x01, 0x43, 0x78, 0x9C, 0x13, 0xEC, 0x48, 0x03, 0x00, 0x01, 0xAC, 0x01, 0x00,
    0x78, 0x9C, 0x63, 0x60, 0xD8, 0xC1, 0xD1, 0xC1, 0x0E, 0x00, 0x04, 0x15, 0x01, 0x50, 0x78, 0x9C,
    0x13, 0xEC, 0x98, 0x01, 0x00, 0x01, 0xDE, 0x01, 0x32, 0x78, 0x9C, 0x13, 0x9C, 0x99, 0x26, 0x0E,
    0x00, 0x02, 0xF6, 0x01, 0x28, 0x78, 0x9C, 0x13, 0x5C, 0x5A, 0x05, 0x00, 0x01, 0xFA, 0x01, 0x31,
    0x78, 0x9C, 0x63, 0x10, 0xE3, 0xE4, 0x10, 0x06, 0x00, 0x00, 0x9B, 0x00, 0x3B, 0x78, 0x9C, 0x63,
    0x61, 0x65, 0x65, 0x02, 0x00, 0x00, 0x2F, 0x00, 0x11, 0x78, 0x9C, 0x63, 0x60, 0x17, 0xE7, 0x60,
    0x01, 0x00, 0x00, 0x7A, 0x00, 0x2B, 0x78, 0x9C, 0x0B, 0x65, 0x52, 0x64, 0x00, 0x00, 0x01, 0xA0,
    0x00, 0x79, 0x78, 0x9C, 0x13, 0x64, 0xD8, 0xE5, 0x75, 0x52, 0x0B, 0x00, 0x05, 0xEE, 0x02, 0x09,
};
const GFXglyph Roboto2BGlyphs[] = {
    { 0, 0, 1, 0, 0, 8, 0 }, //  
    { 1, 4, 1, 0, 3, 12, 8 }, // !
    { 2, 2, 1, 0, 3, 10, 20 }, // "
    { 3, 3, 2, 0, 3, 14, 30 }, // #
    { 3, 5, 2, 0, 4, 18, 44 }, // $
    { 3, 4, 3, 0, 3, 16, 62 }, // %
    { 3, 4, 3, 0, 3, 16, 78 }, // &
    { 1, 2, 1, 0, 3, 10, 94 }, // '
    { 2, 5, 1, 0, 4, 13, 104 }, // (
    { 2, 5, 1, 0, 4, 13, 117 }, // )
    { 2, 2, 2, 0, 3, 10, 130 }, // *
    { 3, 3, 2, 0, 3, 14, 140 }, // +
    { 1, 2, 1, 0, 1, 10, 154 }, // ,
    { 2, 2, 2, 0, 2, 10, 164 }, // -
    { 1, 2, 1, 0, 1, 10, 174 }, // .
    { 3, 4, 2, -1, 3, 16, 184 }, // /
    { 3, 4, 2, 0, 3, 16, 200 }, // 0
    { 2, 3, 2, 0, 3, 11, 216 }, // 1
    { 3, 3, 2, 0, 3, 14, 227 }, // 2
    { 3, 4, 2, 0, 3, 16, 241 }, // 3
    { 3, 3, 2, 0, 3, 14, 257 }, // 4
    { 3, 4, 2, 0, 3, 16, 271 }, // 5
    { 3, 4, 2, 0, 3, 16, 287 }, // 6
    { 3, 3, 2, 0, 3, 14, 303 }, // 7
    { 3, 4, 2, 0, 3, 16, 317 }, // 8
    { 3, 4, 2, 0, 3, 16, 333 }, // 9
    { 1, 4, 1, 0, 3, 12, 349 }, // :
    { 1, 4, 1, 0, 3, 12, 361 }, // ;
    { 2, 3, 2, 0, 3, 11, 373 }, // <
    { 3, 2, 2, 0, 2, 12, 384 }, // =
    { 2, 3, 2, 0, 3, 11, 396 }, // >
    { 2, 4, 2, 0, 3, 12, 407 }, // ?
    { 4, 4, 4, 0, 3, 16, 419 }, // @
    { 3, 3, 3, 0, 3, 14, 435 }, // A
    { 3, 3, 3, 0, 3, 14, 449 }, // B
    { 3, 4, 3, 0, 3, 16, 463 }, // C
    { 3, 3, 3, 0, 3, 14, 479 }, // D
    { 3, 3, 2, 0, 3, 14, 493 }, // E
    { 3, 3, 2, 0, 3, 14, 507 }, // F
    { 3, 4, 3, 0, 3, 16, 521 }, // G
    { 3, 3, 3, 0, 3, 14, 537 }, // H
    { 1, 3, 1, 0, 3, 11, 551 }, // I
    { 2, 4, 2, 0, 3, 12, 562 }, // J
    { 3, 3, 3, 0, 3, 14, 574 }, // K
    { 3, 3, 2, 0, 3, 14, 588 }, // L
    { 4, 3, 4, 0, 3, 14, 602 }, // M
    { 3, 3, 3, 0, 3, 14, 616 }, // N
    { 3, 4, 3, 0, 3, 16, 630 }, // O
    { 3, 3, 3, 0, 3, 14, 646 }, // P
    { 3, 4, 3, 0, 3, 16, 660 }, // Q
    { 3, 3, 3, 0, 3, 14, 676 }, // R
    { 3, 4, 2, 0, 3, 16, 690 }, // S
    { 3, 3, 2, 0, 3, 14, 706 }, // T
    { 3, 4, 3, 0, 3, 16, 720 }, // U
    { 3, 3, 3, 0, 3, 14, 736 }, // V
    { 4, 3, 4, 0, 3, 14, 750 }, // W
    { 3, 3, 3, 0, 3, 14, 764 }, // X
    { 3, 3, 2, 0, 3, 14, 778 }, // Y
    { 3, 3, 2, 0, 3, 14, 792 }, // Z
    { 2, 5, 1, 0, 4, 13, 806 }, // [
    { 2, 4, 2, 0, 3, 12, 819 }, // <backslash>
    { 1, 5, 1, 0, 4, 13, 831 }, // ]
    { 2, 2, 2, 0, 3, 10, 844 }, // ^
    { 2, 1, 2, 0, 0, 9, 854 }, // _
    { 2, 1, 1, 0, 3, 9, 863 }, // `
    { 3, 4, 2, 0, 3, 16, 872 }, // a
    { 3, 4, 2, 0, 3, 14, 888 }, // b
    { 2, 4, 2, 0, 3, 12, 902 }, // c
    { 3, 4, 2, 0, 3, 14, 914 }, // d
    { 3, 4, 2, 0, 3, 16, 928 }, // e
    { 2, 4, 1, 0, 4, 12, 944 }, // f
    { 3, 4, 2, 0, 3, 16, 956 }, // g
    { 3, 3, 2, 0, 3, 14, 972 }, // h
    { 1, 3, 1, 0, 3, 11, 986 }, // i
    { 2, 4, 1, -1, 3, 12, 997 }, // j
    { 3, 3, 2, 0, 3, 14, 1009 }, // k
    { 1, 3, 1, 0, 3, 11, 1023 }, // l
    { 4, 3, 3, 0, 3, 14, 1034 }, // m
    { 3, 3, 2, 0, 3, 14, 1048 }, // n
    { 3, 4, 2, 0, 3, 16, 1062 }, // o
    { 3, 4, 2, 0, 3, 14, 1078 }, // p
    { 3, 4, 2, 0, 3, 14, 1092 }, // q
    { 2, 3, 1, 0, 3, 11, 1106 }, // r
    { 2, 4, 2, 0, 3, 12, 1117 }, // s
    { 2, 4, 1, 0, 3, 12, 1129 }, // t
    { 3, 4, 2, 0, 3, 16, 1141 }, // u
    { 2, 3, 2, 0, 3, 11, 1157 }, // v
    { 3, 3, 3, 0, 3, 14, 1168 }, // w
    { 2, 3, 2, 0, 3, 11, 1182 }, // x
    { 2, 4, 2, 0, 3, 12, 1193 }, // y
    { 2, 3, 2, 0, 3, 11, 1205 }, // z
    { 2, 5, 1, 0, 4, 13, 1216 }, // {
    { 1, 4, 1, 0, 3, 12, 1229 }, // |
    { 2, 5, 1, 0, 4, 13, 1241 }, // }
    { 3, 2, 3, 0, 2, 12, 1254 }, // ~
    { 4, 3, 3, 0, 3, 14, 1266 }, // ₪
};
const UnicodeInterval Roboto2BIntervals[] = {
    { 0x20, 0x7E, 0x0 },
    { 0x20AA, 0x20AA, 0x5F },
};
const GFXfont Roboto2B = {
    (uint8_t*)Roboto2BBitmaps,
    (GFXglyph*)Roboto2BGlyphs,
    (UnicodeInterval*)Roboto2BIntervals,
    2,
    1,
    5,
    4,
    -2,
};