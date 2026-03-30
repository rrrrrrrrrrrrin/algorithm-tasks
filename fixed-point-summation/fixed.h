#ifndef MYFIXED_
#define MYFIXED_
#include <cctype>
#include <cstring>

inline void int_to_str(char*& result, int& i, unsigned int integer) {
  char* temp = new char[12];
  int j = 0;

  if (integer < 0) {
    result[i++] = '0';
  }

  if (integer == 0) {
    temp[j++] = '0';
  } else {
    while (integer > 0) {
      temp[j++] = (integer % 10) + '0';
      integer /= 10;
    }
  }

  temp[j] = '\0';

  // Reverse string
  for (int k = j - 1; k >= 0; --k) {
    result[i++] = temp[k];
  }

  delete[] temp;
}

inline void str_to_int(const char str[], int j, unsigned int& ans) {
  for (int i = 0; i < j; i++) {
    ans = ans * 10 + (str[i] - '0');
  }
}

template <int MAX_DIGITS>
class Fixed {
  signed char sign;
  unsigned long long integer;
  char digits[MAX_DIGITS];  // mantissa

  void normalize() {
    if (integer == 0 && sign < 0) {
      bool isZero = true;

      for (int i = 0; i < MAX_DIGITS; ++i) {
        if (digits[i] != 0) {
          isZero = false;
          break;
        }
      }

      if (isZero) {
        sign = 1;
      }
    }
  }

  Fixed& minus_min_abs(const Fixed& other) {
    char carry = 0;
    for (int i = MAX_DIGITS - 1; i >= 0; --i) {
      digits[i] -= other.digits[i] + carry;
      if (digits[i] < 0) {
        digits[i] += 10;
        carry = 1;
      } else {
        carry = 0;
      }
    }
    integer -= other.integer + carry;
    return *this;
  }

 public:
  Fixed() : sign(1), integer(0) {
    for (int i = 0; i < MAX_DIGITS; ++i) {
      digits[i] = 0;
    }
  }

  Fixed(int num, unsigned int fr = 0) {
    if (num < 0) {
      sign = -1;
      integer = static_cast<unsigned int>(-num);
    } else {
      sign = 1;
      integer = static_cast<unsigned int>(num);
    }

    for (int i = MAX_DIGITS - 1; i >= 0; --i) {
      digits[i] = fr % 10;
      fr /= 10;
    }
  }

  Fixed(unsigned int num) : sign(1), integer(num) {
    for (int i = 0; i < MAX_DIGITS; ++i) {
      digits[i] = 0;
    }
  }

  Fixed(double num) : sign(1) {
    if (num < 0) {
      sign = -1;
      num = -num;
    }

    // Integer part
    integer = static_cast<unsigned int>(num);
    num -= integer;

    // Mantissa part
    for (int i = 0; i < MAX_DIGITS; ++i) {
      num *= 10.0;
      int intnum = static_cast<int>(num);
      digits[i] = static_cast<char>(intnum);
      num -= intnum;
    }
    normalize();
  }

  Fixed(char const str[]) : sign(1), integer(0) {
    int index = 0;

    while (str[index] == ' ') {
      ++index;
    }

    if (str[index] == '-') {
      sign = -1;
      ++index;
    }

    while (str[index] != '\0' && str[index] != '.') {
      if (!std::isdigit(str[index])) {
        break;
      }
      integer = integer * 10 + (str[index] - '0');
      ++index;
    }

    if (str[index] == '.') {
      ++index;
    }

    for (int i = 0; i < MAX_DIGITS; ++i) {
      digits[i] = 0;
    }

    int i = 0;
    while (str[index] != '\0') {
      if (!std::isdigit(str[index])) {
        if (str[index] == 'e') {
          bool shift_left = false;
          char temp[20];

          int j = 0;

          ++index;

          if (str[index] == '-') {
            shift_left = true;
            ++index;
          } else if (str[index] == '+') {
            ++index;
          }

          while (str[index] != '\0' && j < 19) {
            temp[j++] = str[index++];
          }

          unsigned int shift_digits = 0;
          str_to_int(temp, j, shift_digits);

          if (shift_left) {
            this->operator<<=(shift_digits);
          } else {
            this->operator>>=(shift_digits);
          }  // shift_left

          break;
        } else {
          throw "Wrong format";
        }
      }

      if (i < MAX_DIGITS) {
        digits[i++] = str[index++] - '0';
      } else {
        ++index;  // skip digits beyond MAX_DIGITS
      }
    }

    normalize();
  }

  friend class Fixed;

  template <int XXX>
  Fixed(const Fixed<XXX>& other) : sign(other.sign), integer(other.integer) {
    int i = 0;
    for (; i < MAX_DIGITS && i < XXX; ++i) {
      digits[i] = other.digits[i];
    }
    for (; i < MAX_DIGITS; ++i) {
      digits[i] = 0;
    }
  }

  char* to_scientific() const {
    char* result = new char[MAX_DIGITS + 50];
    int i = 0;

    // Handle zero
    bool isZero = (integer == 0);
    if (isZero) {
      for (int j = 0; j < MAX_DIGITS; ++j) {
        if (digits[j] != 0) {
          isZero = false;
          break;
        }
      }
    }

    if (isZero) {
      const char res_zero[25] = "0.000000000000000e+00\0";
      int k = 0;
      while (res_zero[k] != '\0') {
        result[k] = res_zero[k];
        k++;
      }
      return result;
    }

    if (sign < 0) {
      result[i++] = '-';
    }

    if (integer > 0) {
      // Absolute value >= 1.0 (Positive Exponent)
      char intBuf[25];
      int intLen = 0;
      unsigned long long tempInt = integer;

      while (tempInt > 0) {
        intBuf[intLen++] = (tempInt % 10) + '0';
        tempInt /= 10;
      }

      int exp = intLen - 1;

      // Leading digit
      result[i++] = intBuf[intLen - 1];
      result[i++] = '.';

      int printedFractional = 0;

      // Remaining integer digits act as the start of mantissa
      for (int j = intLen - 2; j >= 0 && printedFractional < MAX_DIGITS; --j) {
        result[i++] = intBuf[j];
        printedFractional++;
      }

      // Fractional digits from the array
      for (int j = 0; j < MAX_DIGITS && printedFractional < MAX_DIGITS; ++j) {
        result[i++] = digits[j] + '0';
        printedFractional++;
      }

      // Pad with zeros to reach MAX_DIGITS precision
      while (printedFractional < MAX_DIGITS) {
        result[i++] = '0';
        printedFractional++;
      }

      result[i++] = 'e';
      result[i++] = '+';
      result[i++] = (exp / 10) + '0';
      result[i++] = (exp % 10) + '0';

    } else {
      // Absolute Value < 1.0 (Negative Exponent)
      int firstIdx = 0;
      while (firstIdx < MAX_DIGITS && digits[firstIdx] == 0) {
        firstIdx++;
      }

      // Handle zero
      if (firstIdx == MAX_DIGITS) {
        const char res_zero[25] = "0.000000000000000e+00\0";
        int k = 0;
        while (res_zero[k] != '\0') {
          result[k] = res_zero[k];
          k++;
        }
        return result;
      }

      int exp = -(firstIdx + 1);

      // Leading non-zero digit
      result[i++] = digits[firstIdx] + '0';
      result[i++] = '.';

      int printedFractional = 0;

      // Remaining digits in the array
      for (int j = firstIdx + 1; j < MAX_DIGITS; ++j) {
        result[i++] = digits[j] + '0';
        printedFractional++;
      }

      // Pad with zeros to reach exactly MAX_DIGITS precision
      while (printedFractional < MAX_DIGITS) {
        result[i++] = '0';
        printedFractional++;
      }

      result[i++] = 'e';
      result[i++] = '-';
      int absExp = -exp;
      result[i++] = (absExp / 10) + '0';
      result[i++] = (absExp % 10) + '0';
    }

    result[i] = '\0';
    return result;
  }

  char* to_string() const {
    char* result =
        new char[MAX_DIGITS +
                 50];  // MAX_DIGITS + unsigned long long (20) + extra
    int i = 0;

    if (sign < 0) {
      result[i++] = '-';
    }

    int_to_str(result, i, integer);  // result appends integer

    result[i++] = '.';

    for (int j = 0; j < MAX_DIGITS; ++j) {
      result[i++] = digits[j] + '0';
    }

    result[i] = '\0';

    return result;
  }

  double to_double() const {
    double num = double(integer);
    double divisor = 10.0;

    for (int i = 0; i < MAX_DIGITS; ++i) {
      num += digits[i] / divisor;
      divisor *= 10.0;
    }

    return sign < 0 ? -num : num;
  }

  Fixed& operator+=(const Fixed& other) {
    if (sign == other.sign) {
      char carry = 0;
      for (int i = MAX_DIGITS - 1; i >= 0; --i) {
        digits[i] += other.digits[i] + carry;

        if (digits[i] > 9) {
          digits[i] -= 10;
          carry = 1;
        } else {
          carry = 0;
        }
      }
      integer += other.integer + carry;
    } else {
      if (this->abs() >= other.abs()) {
        minus_min_abs(other);
      } else {
        Fixed tmp(other);
        *this = tmp.minus_min_abs(*this);
      }
    }
    normalize();
    return *this;
  }

  // Left shift of the dot (Fixed decreases)
  Fixed& operator<<=(int shift) {
    if (shift <= 0) {
      return *this;
    }

    // Move digits to the right by shift
    for (int i = MAX_DIGITS - 1; i >= shift; --i) {
      digits[i] = digits[i - shift];
    }

    // Insert integer's digits into digits
    for (int i = shift - 1; i >= 0; --i) {
      if (i < MAX_DIGITS) {
        digits[i] = integer % 10;
      }
      integer /= 10;
    }

    return *this;
  }

  // Right shift of the dot (Fixed increases)
  Fixed& operator>>=(int shift) {
    if (shift <= 0) {
      return *this;
    }

    // Move digits to the left by shift into integer
    for (int i = 0; i < shift && i < MAX_DIGITS; ++i) {
      if (i < MAX_DIGITS) {
        integer = integer * 10 + digits[i];
      } else {
        integer = integer * 10;
      }
    }

    int i = 0;
    // Move digits to the left by shift
    for (; i < MAX_DIGITS - shift; i++) {
      digits[i] = digits[i + shift];
    }

    // Fill remaining space at end with zeros
    for (; i < MAX_DIGITS; i++) {
      digits[i] = 0;
    }

    return *this;
  }

  Fixed abs() const { return sign < 0 ? -*this : *this; }

  bool is_equal(const Fixed& other) const {
    bool answer = ((sign == other.sign) && (integer == other.integer));
    if (answer) {
      for (int i = 0; i < MAX_DIGITS; ++i) {
        if (digits[i] != other.digits[i]) {
          answer = false;
          break;
        }
      }
    }
    return answer;
  }

  bool is_less(const Fixed& other) const {
    if (sign < other.sign) {
      return true;
    }
    if (other.sign < sign) {
      return false;
    }

    if (sign < 0) {
      return (-other).is_less(-(*this));
    }

    if (integer < other.integer) {
      return true;
    }
    if (other.integer < integer) {
      return false;
    }

    for (int i = 0; i < MAX_DIGITS; ++i) {
      if (digits[i] < other.digits[i]) {
        return true;
      }
      if (other.digits[i] < digits[i]) {
        return false;
      }
    }
    return false;
  }

  Fixed operator-() const {
    Fixed result(*this);
    result.sign = -result.sign;
    result.normalize();
    return result;
  }
};

template <int MAX_DIGITS>
Fixed<MAX_DIGITS> operator+(const Fixed<MAX_DIGITS>& left,
                            const Fixed<MAX_DIGITS>& right) {
  Fixed<MAX_DIGITS> result(left);
  result += right;
  return result;
}

template <int MAX_DIGITS>
Fixed<MAX_DIGITS> operator-(const Fixed<MAX_DIGITS>& left,
                            const Fixed<MAX_DIGITS>& right) {
  return left + (-right);
}

template <int MAX_DIGITS>
inline bool operator<(const Fixed<MAX_DIGITS>& left,
                      const Fixed<MAX_DIGITS>& right) {
  return left.is_less(right);
}

template <int MAX_DIGITS>
inline bool operator==(const Fixed<MAX_DIGITS>& left,
                       const Fixed<MAX_DIGITS>& right) {
  return left.is_equal(right);
}

template <int MAX_DIGITS>
inline bool operator>(const Fixed<MAX_DIGITS>& left,
                      const Fixed<MAX_DIGITS>& right) {
  return right < left;
}

template <int MAX_DIGITS>
inline bool operator<=(const Fixed<MAX_DIGITS>& left,
                       const Fixed<MAX_DIGITS>& right) {
  // return (left < right) || (left==right);
  return !(right < left);
}

template <int MAX_DIGITS>
inline bool operator>=(const Fixed<MAX_DIGITS>& left,
                       const Fixed<MAX_DIGITS>& right) {
  return !(left < right);
}

template <int MAX_DIGITS>
inline bool operator!=(const Fixed<MAX_DIGITS>& left,
                       const Fixed<MAX_DIGITS>& right) {
  return !(left == right);
}

#endif