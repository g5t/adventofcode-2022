#ifndef AOC_HOTAIR_H
#define AOC_HOTAIR_H
namespace aoc::hotair {
using ind_t = int64_t;

constexpr ind_t pow5[] {
                   1,
                   5,
                  25,
                 125,
                 625,
                3125,
               15625,
               78125,
              390625,
             1953125,
             9765625,
            48828125,
           244140625,
          1220703125,
          6103515625,
         30517578125,
        152587890625,
        762939453125,
       3814697265625,
      19073486328125,
      95367431640625,
     476837158203125,
    2384185791015625,
   11920928955078125,
   59604644775390625,
  298023223876953125,
 1490116119384765625,
 7450580596923828125
 };

constexpr ind_t biggest_base5 = 
  pow5[0]+pow5[1]+pow5[2]+pow5[3]+pow5[4]+pow5[5]+pow5[6]+pow5[7]+pow5[8]+pow5[9]+
  pow5[10]+pow5[11]+pow5[12]+pow5[13]+pow5[14]+pow5[15]+pow5[16]+pow5[17]+pow5[18]+pow5[19]+
	pow5[20]+pow5[21]+pow5[22]+pow5[23]+pow5[24]+pow5[25]+pow5[26];

constexpr size_t max_pow5 = 27;

class SNAFU {
  ind_t dec_=0;
public:
  explicit SNAFU(ind_t dec): dec_(dec) {}
  explicit SNAFU(std::string fem){
		for (size_t i=0; i<fem.size(); ++i){
			ind_t n{0};
			switch (fem[i]){
			case '2': n= 2; break;
			case '1': n= 1; break;
			case '0': n= 0; break;
			case '-': n=-1; break;
			case '=': n=-2; break;
			default: throw std::runtime_error("This is not a SNAFU number!");
			}
			dec_ += n * pow5[fem.size()-1-i];
		}
	}

	ind_t dec() const {return dec_;}

	SNAFU operator+(const SNAFU & s) const {return SNAFU(dec_+s.dec());}
	std::weak_ordering operator<=>(const SNAFU & s) const {return dec_ <=> s.dec();}

	std::string to_string() const {
		size_t num_digits{0};
		for (size_t i=max_pow5; i-->0;) if (dec_ <= 2 * pow5[i]) num_digits=i;
		// offset by - (========...===)
		ind_t remaining{dec_};
		for (size_t i=0; i<num_digits+1; ++i) remaining += 2 * pow5[i];
		if (remaining > biggest_base5) std::cout << dec_ << " is too big to be a SNAFU!\n";

		// find the normal base-5 representation
		std::vector<ind_t> digits(max_pow5+1, 2);
		for (size_t i=max_pow5; i-->0;){
		  auto num = remaining / pow5[i];
			if (num >= 0) {
			  digits[i] = num;
				remaining -= num * pow5[i];
			}
		}
		// and covert it to the SNAFU representation
		std::stringstream ss;
		bool printing{false};
		for (size_t i=max_pow5; i-->0;){
		  if (!printing && digits[i] > 2) printing=true;
			if (printing) switch (digits[i]) {
			case 0: ss << '='; break;
			case 1: ss << '-'; break;
			case 2: ss << '0'; break;
			case 3: ss << '1'; break;
			case 4: ss << '2'; break;
			default: 
			  std::cout << "Dont know what to do with digits[" << i << "] = " << digits[i] << "\n";
			  throw std::runtime_error("Base 5 numerical error");
	    }
		}
		return ss.str();
	}
  friend std::ostream & operator<<(std::ostream & os, const SNAFU & s){
	  os << s.to_string();
		return os;
	}
};

}; // soc::hotair
#endif
