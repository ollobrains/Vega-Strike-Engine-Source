/*
 * Copyright (C) 2001-2022 Daniel Horn, pyramid3d, Stephen G. Tuggy,
 * and other Vega Strike contributors.
 *
 * https://github.com/vegastrike/Vega-Strike-Engine-Source
 *
 * This file is part of Vega Strike.
 *
 * Vega Strike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Vega Strike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Vega Strike. If not, see <https://www.gnu.org/licenses/>.
 */

#include "product.h"

static const std::string default_product_name("DEFAULT_PRODUCT_NAME");

Product::Product(): name_(default_product_name), quantity_(1), unit_price_(0.0) {}


Product::Product(const std::string &name, const double quantity, const double unit_price) :
    name_(name), quantity_(quantity), unit_price_(unit_price) {}


double Product::add(double quantity)
{
    return 0.0;
}


double Product::subtract(double quantity)
{
    return 0.0;
}


bool operator==(const Product &lhs, const std::string &rhs) {
    return lhs.name_ == rhs;
}

bool operator==(const std::string &lhs, const Product &rhs) {
    return lhs == rhs.name_;
}


bool operator!=(const Product &lhs, const std::string &rhs) {
    return lhs.name_ != rhs;
}

bool operator!=(const std::string &lhs, const Product &rhs) {
    return lhs != rhs.name_;
}
