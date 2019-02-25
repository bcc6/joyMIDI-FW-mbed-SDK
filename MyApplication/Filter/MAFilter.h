#ifndef MAFILTER_H
#define MAFILTER_H

#include "mbed.h"

/** Digital filter - Moving Average Filter
 *
 * Example:
 * @code
 * // Print messages the original sample and the sample after the MA-filter
 *
 * #include "MAFilter.h"
 *
 * AnalogIn ain(A0);
 * MAFilter<uint16_t, uint32_t> maf(8);
 *
 * int main() {
 *     while(1) {
 *         uint16_t val_ori = ain.read();
 *         uint16_t val_maf = maf.add(val_ori).average();
 *         printf("%d,%d\n", val_ori, val_maf);
 *     }
 * }
 * @endcode
 */

template <typename SampleType, typename SumType>
class MAFilter {
public:
    MAFilter(uint8_t lenth) : _samples(0), _sum(0), _lenth(lenth), _count(0), _is_full(false) {
        _samples = new SampleType[lenth];
    }
    
    MAFilter& add(SampleType sample) {
        if (! _is_full) {
            _samples[_count++] = sample;
            _sum += sample;
            _is_full = (_count >= _lenth) ? true : false;
        } else {
            SampleType &oldest = _samples[_count++ % _lenth];
            _sum += sample - oldest;
            oldest = sample;
        }
		return *this;
    }
    
    SampleType average() {
        return _is_full ? (_sum / _lenth) : (_sum / _count);
    }
    
private:
    SampleType *_samples;
    SumType     _sum;
    uint8_t     _lenth;
    uint8_t     _count;
    bool        _is_full;
};

#endif