#ifndef INCLUDED_REED_SOLOMON_H
#define INCLUDED_REED_SOLOMON_H

#include <ccsds/api.h>
#include <stdint.h>

namespace gr {
    namespace ccsds {

        class CCSDS_API reed_solomon {
            private:
            public:
                reed_solomon();
                ~reed_solomon();

                void encode(uint8_t *data);
                int16_t decode(uint8_t *data);
        };

    }
}

#endif /* INCLUDED_REED_SOLOMON_H */
