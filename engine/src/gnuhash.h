/**
* gnuhash.h
*
* Copyright (c) 2001-2002 Daniel Horn
* Copyright (c) 2002-2019 pyramid3d and other Vega Strike Contributors
* Copyright (c) 2019-2021 Stephen G. Tuggy, and other Vega Strike Contributors
*
* https://github.com/vegastrike/Vega-Strike-Engine-Source
*
* This file is part of Vega Strike.
*
* Vega Strike is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
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

#ifndef _GNUHASH_H_
#define _GNUHASH_H_
#include <unordered_map>

#define vsUMap     std::unordered_map
#define vsHashComp std::hash_compare
#define vsHash     std::hash
class Unit;

namespace std
{

// defining things in std:: for basic types is ... frowned upon: http://eel.is/c++draft/namespace.std#1
template < >
struct hash< void* >
{
    hash< size_t >a;
public:
    size_t operator()( const void *key ) const
    {
        return a( (size_t) key );
    }
};
template < >
struct hash< const void* >
{
    hash< size_t >a;
public:
    size_t operator()( const void*const &key ) const
    {
        return a( (size_t) key );
    }
};

template < >
struct hash< const Unit* >
{
    hash< size_t >a;
public:
    size_t operator()( const Unit*const &key ) const
    {
        return a( (size_t) key>>4 );
    }
};
template < >
struct hash< std::pair< Unit*, Unit* > >
{
    hash< size_t >a;
public:
    size_t operator()( const std::pair< Unit*, Unit* > &key ) const
    {
        return (size_t) ((size_t) ( a( (int) ( ( (size_t) key.first )>>4 ) ))
                                  ^((size_t) a( (int) ( ( (size_t) key.second )>>4 ) ) ));
    }
};
#ifdef __GNUC__
//Minimum declaration needed by SharedPool.h
template < class Key, class Traits = std::less< Key > >
class hash_compare
{
public:
    static const size_t bucket_size = 4;
    static const size_t min_buckets = 8;
};
#endif
}

#endif //def _GNUHASH_H_

