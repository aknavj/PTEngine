#ifndef __RANDOM_H__
#define __RANDOM_H__

/*
 */
#define BITS		32
#define MSB			0x80000000L
#define ALL_BITS	0xffffffffL
#define HALF_RANGE	0x40000000L
#define STEP		7

/*
 */
class CRandom {

	protected:
		long m_nSeed;
		long m_nQuotient;
		long m_nRemainder;

	public:

		/*
		 */
		CRandom() { }
		CRandom(unsigned int nSeed)	{ Init(nSeed); }

		/*
		*/
		void Init(unsigned int nSeed) { srand(nSeed); }
		double Random()	{ return (double)rand() / (double)RAND_MAX; }

		/*
		*/
		double RandomD(double dMin, double dMax) {
			double dInterval = dMax - dMin;
			double d = dInterval * Random();
			return dMin + MathLib::Min(d, dInterval);
		}

		/*
		*/
		unsigned int RandomI(unsigned int nMin, unsigned int nMax) {
			unsigned int nInterval = nMax - nMin;
			unsigned int i = (unsigned int)((nInterval + 1.0) * Random());
			return nMin + MathLib::Min(i, nInterval);
		}
};

/*
*/
class CRandomLCG {

	protected:
		unsigned int m_nSeed;
		unsigned int m_nQuotient;
		unsigned int m_nRemainder;

	public:

		/*
		*/
		CRandomLCG(unsigned int nSeed) {
			m_nSeed = nSeed;
			m_nQuotient = LONG_MAX / 16807L;
			m_nRemainder = LONG_MAX % 16807L;
		}

		/*
		*/
		unsigned int rand() {
			if (m_nSeed <= m_nQuotient) m_nSeed = (m_nSeed * 16807L) % LONG_MAX;
			else {
				int nHigh = m_nSeed / m_nQuotient;
				int nLow = m_nSeed % m_nQuotient;

				int test = 16807L * nLow - m_nRemainder * nHigh;

				if (test > 0) m_nSeed = test;
				else m_nSeed = test + LONG_MAX;
			}
			return m_nSeed;
		}
};

/*
*/
class CRandomR250 {

	protected:
		unsigned int m_nR250buffer[250];
		int m_nR250index;

	public:

		/*
		 */
		CRandomR250(unsigned int nSeed) {
			CRandomLCG lcg(nSeed);

			int j, k;
			unsigned int mask, msb;
			m_nR250index = 0;
			for (j = 0; j < 250; j++) m_nR250buffer[j] = rand();

			for (j = 0; j < 250; j++) {
				if (lcg.rand() > HALF_RANGE) m_nR250buffer[j] |= MSB;
			}

			msb = MSB;	        // Zapne uhlopøíèný bit
			mask = ALL_BITS;	// vypne krajní bity

			for (j = 0; j < BITS; j++) {
				k = STEP * j + 3;
				m_nR250buffer[k] &= mask;
				m_nR250buffer[k] |= msb;
				mask >>= 1;
				msb >>= 1;
			}
		}

		/*
		 */
		unsigned int r250() {
			register int j;
			register unsigned int new_rand;

			if (m_nR250index >= 147) j = m_nR250index - 147;
			else j = m_nR250index + 103;

			new_rand = m_nR250buffer[m_nR250index] ^ m_nR250buffer[j];
			m_nR250buffer[m_nR250index] = new_rand;

			if (m_nR250index >= 249) m_nR250index = 0;
			else m_nR250index++;

			return new_rand;
		}

		/*
		 */
		double dr250() {
			register int j;
			register unsigned int new_rand;

			if (m_nR250index >= 147) j = m_nR250index - 147;
			else j = m_nR250index + 103;

			new_rand = m_nR250buffer[m_nR250index] ^ m_nR250buffer[j];
			m_nR250buffer[m_nR250index] = new_rand;

			if (m_nR250index >= 249) m_nR250index = 0;
			else m_nR250index++;

			return (double)new_rand / ALL_BITS;
		}
};

#endif /* __RANDOM_H__ */