#pragma once
#include "includes.h"

int eval(chess::Board &position) {
    int score = 0;
    score += 100 * (
        chess::builtin::popcount(position.pieces(chess::PieceType::Pawn, chess::Color::White)) -
        chess::builtin::popcount(position.pieces(chess::PieceType::Pawn, chess::Color::Black)));
    score += 300 * (
        chess::builtin::popcount(position.pieces(chess::PieceType::Knight, chess::Color::White)) -
        chess::builtin::popcount(position.pieces(chess::PieceType::Knight, chess::Color::Black)));
    score += 320 * (
        chess::builtin::popcount(position.pieces(chess::PieceType::Bishop, chess::Color::White)) -
        chess::builtin::popcount(position.pieces(chess::PieceType::Bishop, chess::Color::Black)));
    score += 500 * (
        chess::builtin::popcount(position.pieces(chess::PieceType::Rook, chess::Color::White)) -
        chess::builtin::popcount(position.pieces(chess::PieceType::Rook, chess::Color::Black)));
    score += 900 * (
        chess::builtin::popcount(position.pieces(chess::PieceType::Queen, chess::Color::White)) -
        chess::builtin::popcount(position.pieces(chess::PieceType::Queen, chess::Color::Black)));
    
    if(position.sideToMove() == chess::Color::Black) return -score;
    return score;
}