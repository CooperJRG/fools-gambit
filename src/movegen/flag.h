#pragma once
#include <cstdint>

enum class MoveFlag : uint8_t {
    Quiet      = 0,
    DoublePush = 1,
    KingCastle = 2,
    QueenCastle = 3,
    Capture    = 4,
    EnPassant  = 5,
    KnightPromotion = 6,
    BishopPromotion = 7,
    RookPromotion   = 8,
    QueenPromotion  = 9,
    KnightPromotionCapture = 10,
    BishopPromotionCapture = 11,
    RookPromotionCapture   = 12,
    QueenPromotionCapture  = 13,
};
