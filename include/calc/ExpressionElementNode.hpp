#pragma once
struct ExpressionElementNode 
{
    virtual double value() = 0;
    virtual ~ExpressionElementNode() = default;
};
