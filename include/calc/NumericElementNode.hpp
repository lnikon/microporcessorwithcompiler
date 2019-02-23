#pragma once
#include "ExpressionElementNode.hpp"

struct NumericElementNode : ExpressionElementNode
{
    NumericElementNode(const NumericElementNode&) = delete;
    NumericElementNode() = delete;
    NumericElementNode& operator=(const NumericElementNode&) = delete;

    NumericElementNode(double value);
    virtual double value() override;

    private:
    double m_number;
};
