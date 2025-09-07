
#ifndef POS_H
#define POS_H

#include <cstddef>
#include <bitset>

struct pos2
{
    int x, y;
    pos2() = default; // 添加默认构造函数
    pos2(int x_val, int y_val) : x(x_val), y(y_val) {}
};

inline bool operator==(const pos2 &lhs, const pos2 &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
inline bool operator!=(const pos2 &lhs, const pos2 &rhs)
{
    return !(lhs == rhs);
}
inline pos2 operator+(const pos2 &lhs, const pos2 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline pos2 operator-(const pos2 &lhs, const pos2 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

struct pos2f
{
    float x, y;

    pos2f(const pos2 &p) : x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) {}

    // Other constructors and operators as needed
    pos2f(float x_val, float y_val) : x(x_val), y(y_val) {}
};

inline bool operator==(const pos2f &lhs, const pos2f &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
inline pos2f operator+(const pos2f &lhs, const pos2f &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline pos2f operator-(const pos2f &lhs, const pos2f &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

struct pos2d
{
    double x, y;
    pos2d() = default; // 添加默认构造函数
    pos2d(const pos2 &p) : x(static_cast<double>(p.x)), y(static_cast<double>(p.y)) {}

    // Other constructors and operators as needed
    pos2d(double x_val, double y_val) : x(x_val), y(y_val) {}
};

inline bool operator==(const pos2d &lhs, const pos2d &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
inline pos2d operator+(const pos2d &lhs, const pos2d &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
inline pos2d operator+(const pos2d &lhs, const double rhs)
{
    return {lhs.x + rhs, lhs.y + rhs};
}
inline pos2d operator+(const double rhs, const pos2d &lhs)
{
    return {lhs.x + rhs, lhs.y + rhs};
}
inline pos2d operator-(const pos2d &lhs, const pos2d &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
inline pos2d operator*(const pos2d &lhs, const pos2d &rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}

struct pos3
{
    int x, y, z;
    pos3() = default; // 添加默认构造函数
    pos3(int x_val, int y_val, int z_val) : x(x_val), y(y_val), z(z_val) {}
};

inline bool operator==(const pos3 &lhs, const pos3 &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
inline pos3 operator+(const pos3 &lhs, const pos3 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
inline pos3 operator-(const pos3 &lhs, const pos3 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

struct pos3d
{
    double x, y, z;
    pos3d(double x_val, double y_val, double z_val) : x(x_val), y(y_val), z(z_val) {}

    pos3d(const pos3 &p) : x(static_cast<double>(p.x)), y(static_cast<double>(p.y)), z(static_cast<double>(p.z)) {}
    inline pos3d &operator+=(const double value)
    {
        x += value;
        y += value;
        z += value;
        return *this;
    }
    pos3d() = default; // 添加默认构造函数
};

inline bool operator==(const pos3d &lhs, const pos3d &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
inline pos3d operator+(const pos3d &lhs, const pos3d &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
inline pos3d operator+(const pos3d &lhs, const double rhs)
{
    return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};
}
inline pos3d operator-(const pos3d &lhs, const pos3d &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
inline pos3d operator-(const pos3d &lhs, const double &rhs)
{
    return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
}
inline pos3d operator*(const pos3d &lhs, const pos3d &rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
}

struct pos4
{
    int x, y, z, w;

    pos4() = default;
    pos4(int tx) : x(tx), y(tx), z(tx), w(tx) {}
    pos4(int x_val, int y_val, int z_val, int w_val) : x(x_val), y(y_val), z(z_val), w(w_val) {}
    const int &operator[](std::size_t index) const
    {
        // 类似上面的实现
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            throw std::out_of_range("Index out of range");
        }
    }
};

inline bool operator==(const pos4 &lhs, const pos4 &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
}
inline pos4 operator+(const pos4 &lhs, const pos4 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}
inline pos4 operator-(const pos4 &lhs, const pos4 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

struct pos4f
{
    float x, y, z, w;

    const float &operator[](std::size_t index) const
    {
        // 类似上面的实现
        switch (index)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        default:
            throw std::out_of_range("Index out of range");
        }
    }
};

inline bool operator==(const pos4f &lhs, const pos4f &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
}
inline pos4f operator+(const pos4f &lhs, const pos4f &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}
inline pos4f operator-(const pos4f &lhs, const pos4f &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

struct pos3u
{
    std::bitset<4> x, y, z;
};
inline std::bitset<4> operator+(const std::bitset<4> &lhs, const std::bitset<4> &rhs)
{
    std::bitset<4> result;
    bool carry = false;

    for (int i = 0; i < 4; ++i)
    {
        bool bit_a = lhs[i];
        bool bit_b = rhs[i];

        // 加法操作
        result[i] = bit_a ^ bit_b ^ carry;

        // 计算进位
        carry = (bit_a & bit_b) | (carry & (bit_a ^ bit_b));
    }
    return result;
}
inline std::bitset<4> operator+(const std::bitset<4> &lhs, const int &rhs)
{
    std::bitset<4> lhs2(rhs);
    return lhs + lhs2;
}

inline std::bitset<4> operator-(const std::bitset<4> &lhs, const std::bitset<4> &rhs)
{
    return lhs + ~rhs + std::bitset<4>(1);
}
inline std::bitset<4> operator-(const std::bitset<4> &lhs, const int &rhs)
{
    std::bitset<4> lhs2(rhs);
    return lhs - lhs2;
}

inline std::bitset<4> operator&(const std::bitset<4> &lhs, const int &rhs)
{
    std::bitset<4> lhs2(rhs);
    return lhs & lhs2;
}

inline bool operator==(const pos3u &lhs, const pos3u &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}
inline pos3u operator+(const pos3u &lhs, const pos3u &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
inline pos3u operator-(const pos3u &lhs, const pos3u &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

#endif // POS_H