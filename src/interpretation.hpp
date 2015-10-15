#ifndef INTERPRETATION_HPP_SEEN
#define INTERPRETATION_HPP_SEEN

namespace nut
{
	// A ThreeVector instantiated with Interpretation::VOID can't be multiplied by a
	// ModelViewMatrix.  Interpretation::VERTEX results in a multiplication
	// interpretating the elements of the vector as homogeneous coordinates with w being
	// impplicitly 1.  Interpretation::NORMAL uses 0.
	enum class Interpretation : unsigned char{VOID, VERTEX, NORMAL};

	const Interpretation VOID   = Interpretation::VOID;
	const Interpretation VERTEX = Interpretation::VERTEX;
	const Interpretation NORMAL = Interpretation::NORMAL;
}

#endif //INTERPRETATION_HPP_SEEN

// vim: tw=90 ts=2 sts=-1 sw=0 noet
