////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     ConvolutionTestData.cpp (dsp)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ConvolutionTestData.h"

// math
#include "Matrix.h"
#include "Vector.h"

// utilities
#include "Exception.h"

// stl
#include <algorithm>
#include <vector>
namespace
{
    // Helper function to avoid annoying double-to-float errors
    template <typename ValueType, typename ValueType2>
    ell::math::RowMatrix<ValueType> MakeMatrix(std::initializer_list<std::initializer_list<ValueType2>> list)
    {
        auto numRows = list.size();
        auto numColumns = list.begin()->size();
        std::vector<ValueType> data;
        data.reserve(numRows * numColumns);
        for(const auto& row: list)
        {
            DEBUG_THROW(row.size() != numColumns, ell::utilities::InputException(ell::utilities::InputExceptionErrors::sizeMismatch, "incorrect number of elements in initializer list"));
            std::transform(row.begin(), row.end(), std::back_inserter(data), [](ValueType2 x) { return static_cast<ValueType>(x); });
        }
        return ell::math::RowMatrix<ValueType>(numRows, numColumns, data);
    }
}

//
// NOTE: The examples matrices below were randomly-generated using
//       the following script:
//
// ```
// import numpy as np
// import scipy
// import scipy.signal
//
// signal = np.random.rand(14,14)
// filter = np.array([[0.25, 0.5, 0.25], [0.5, 0.75, 0.5], [0.25, 0.5, 0.25]])
// reference = scipy.signal.correlate2d(signal, filter, 'valid')
// ```

// This is `signal` from the above script
template <typename ValueType>
ell::math::RowMatrix<ValueType> GetReferenceMatrixSignal()
{
    // clang-format off
    return MakeMatrix<ValueType>({ { 0.54900258127, 0.782928093357, 0.954594952519, 0.817351111922, 0.792785972612, 0.25462638477, 0.210152585739, 0.692073223247, 0.167481157006, 0.971090467053, 0.179318733006, 0.599021152946, 0.834673554887, 0.386348427793 },
                { 0.524057667387, 0.194810273283, 0.281117429801, 0.85840343111, 0.341948878426, 0.746582556421, 0.652652661702, 0.128009583894, 0.0518214129825, 0.725093613108, 0.959391654627, 0.855865690685, 0.477288810167, 0.377122018536 },
                { 0.108184051901, 0.33939357211, 0.226753457819, 0.850992043036, 0.681719017611, 0.0533975345706, 0.42068870679, 0.464879372521, 0.218378069544, 0.687925640618, 0.926972195288, 0.588161280455, 0.411787003919, 0.859107295273 },
                { 0.108170633046, 0.184432925953, 0.439135204701, 0.136856091125, 0.950697274789, 0.333354459857, 0.111668226374, 0.531076395508, 0.633729543749, 0.75789003677, 0.127282417915, 0.519996210482, 0.193265765535, 0.413893963557 },
                { 0.405355941997, 0.0950991780739, 0.164723529435, 0.936468204574, 0.712509234607, 0.170323650516, 0.566201286972, 0.903150190614, 0.189523706754, 0.249848622496, 0.390054680957, 0.851004127194, 0.734241221245, 0.462824060854 },
                { 0.246433071702, 0.58417884217, 0.308506836744, 0.952180441067, 0.0826696252869, 0.676169525361, 0.578593132235, 0.0810779857816, 0.331668812888, 0.697850622984, 0.386628990663, 0.372877165672, 0.998427170653, 0.107866013788 },
                { 0.0299082072365, 0.382381734032, 0.0596806245826, 0.539939025508, 0.285417652144, 0.662150329379, 0.854457402021, 0.222442083267, 0.72315296536, 0.690698845261, 0.748972642195, 0.923393363494, 0.132076199003, 0.389961436485 },
                { 0.332783177986, 0.562495817466, 0.500071685344, 0.757689090871, 0.241922473758, 0.145234055309, 0.492420835659, 0.895944662555, 0.242841851215, 0.725342041514, 0.114875246485, 0.834584325333, 0.480517797147, 0.341442834269 },
                { 0.485121811659, 0.391626481473, 0.172981597932, 0.659609750312, 0.87355251774, 0.295062993209, 0.227623088872, 0.224934255689, 0.960848231264, 0.73226560645, 0.0338050629986, 0.0374959935251, 0.265226166829, 0.638005942771 },
                { 0.947887889768, 0.312518716012, 0.278163416621, 0.50055266913, 0.276198498014, 0.792436717921, 0.0388150143841, 0.581632337858, 0.234726422424, 0.519516205948, 0.652418239305, 0.668627788659, 0.310788592851, 0.893640561719 },
                { 0.893814399984, 0.882275183903, 0.685568800809, 0.295184012537, 0.355057346011, 0.454044965115, 0.53686768799, 0.351192570039, 0.0526844279268, 0.545094401901, 0.747187579428, 0.339875842775, 0.0414601104538, 0.444100500321 },
                { 0.565638536702, 0.959288701095, 0.366378294843, 0.43723500187, 0.731930019491, 0.376659247804, 0.282952323504, 0.901333137561, 0.996510208218, 0.486521231902, 0.868010841121, 0.560350373095, 0.264986434773, 0.5045245095 },
                { 0.706167270526, 0.474539951508, 0.241097554444, 0.798402707577, 0.179302385337, 0.52959458882, 0.590398761492, 0.740271943116, 0.914181113993, 0.200945537262, 0.874376404797, 0.955201845254, 0.0457408008773, 0.694626338824 },
                { 0.230886991943, 0.356406367169, 0.440483993748, 0.586040753365, 0.70800182598, 0.295639566469, 0.323260206703, 0.905979251874, 0.157441568393, 0.757045347332, 0.115188898905, 0.938322799919, 0.395105943031, 0.630054988039 } });
    // clang-format on
}

// This is `filter` from the above script
template <typename ValueType>
ell::math::RowMatrix<ValueType> GetReferenceMatrixFilter()
{
    // clang-format off
    return MakeMatrix<ValueType>(
           { { 0.25, 0.5, 0.25 },
             { 0.5, 0.75, 0.5 },
             { 0.25, 0.5, 0.25 } });
    // clang-format on
}

// This is `reference` from the above script
template <typename ValueType>
ell::math::RowMatrix<ValueType> GetReferenceMatrixConvolution()
{
    // clang-format off
    return MakeMatrix<ValueType>({ { 1.56948984717, 2.02578533482, 2.45347065507, 2.29029891627, 1.73758621774, 1.60845034148, 1.28089565292, 1.36233944737, 2.25197233618, 2.77471849228, 2.54201858889, 2.20585028076 },
            { 0.949755767294, 1.46900989856, 2.09333472804, 2.12860626325, 1.64546228159, 1.3915711766, 1.36020386846, 1.61847894441, 2.27316744514, 2.59132103103, 2.23773867259, 1.90947129947 },
            { 0.855478233718, 1.24122265514, 2.13771474176, 2.14803771587, 1.48833874951, 1.4073492814, 1.80371890578, 1.90018221866, 1.8490428031, 1.98715329288, 1.88561772876, 1.8751827147 },
            { 1.01623143988, 1.4775594342, 2.13073803693, 2.12910143289, 1.70276705596, 1.71193815672, 1.77521725928, 1.7183151252, 1.57487343191, 1.68707659883, 2.07323882077, 2.15709981643 },
            { 1.11926161772, 1.60023388173, 1.95350993705, 1.95236094805, 1.85864190666, 2.01241451269, 1.66206943911, 1.61108918552, 1.86573660171, 2.07358553851, 2.36072088611, 2.07914642485 },
            { 1.2518667392, 1.62434615714, 1.71573082875, 1.61022224423, 1.82615358119, 2.06825279904, 1.85552922831, 1.88624434854, 2.23468699529, 2.27719124347, 2.23191248996, 1.90939961791 },
            { 1.41222646287, 1.64486657726, 1.88694638371, 1.75157903795, 1.51496652268, 1.78209264651, 1.95479843171, 2.30236053642, 2.0510420313, 1.85347192307, 1.69909946208, 1.64426729465 },
            { 1.57500537507, 1.57778593869, 1.971184269, 1.94053937926, 1.50305964028, 1.30014580954, 1.75392588296, 2.11862905331, 1.98017041556, 1.48089893049, 1.318893636, 1.61689766576 },
            { 2.04373717549, 1.60160731146, 1.65178240621, 1.8939239292, 1.62466343395, 1.42969987312, 1.30556424367, 1.69675712684, 1.9205208149, 1.88756495916, 1.44367990565, 1.53243832832 },
            { 2.62681873136, 1.97757582659, 1.62376247555, 1.67169266627, 1.70351818755, 1.6291685628, 1.68790421533, 1.72552585055, 1.99969195179, 2.32184424702, 1.78777083491, 1.36775657644 },
            { 2.49554451574, 2.04897921412, 1.78913029028, 1.74238107364, 1.69716192953, 1.93361967682, 2.38499637346, 2.38411872493, 2.31727879971, 2.49550533225, 2.06149148567, 1.38322885484 },
            { 1.88823186456, 1.80546834555, 1.88233841171, 1.94233500231, 1.62973201609, 2.00074140312, 2.6511911665, 2.49594020616, 2.20105908112, 2.41101580293, 2.33661960274, 1.84757904924 } });
    // clang-format on
}

template ell::math::RowMatrix<float> GetReferenceMatrixFilter();
template ell::math::RowMatrix<double> GetReferenceMatrixFilter();

template ell::math::RowMatrix<float> GetReferenceMatrixSignal();
template ell::math::RowMatrix<double> GetReferenceMatrixSignal();

template ell::math::RowMatrix<float> GetReferenceMatrixConvolution();
template ell::math::RowMatrix<double> GetReferenceMatrixConvolution();