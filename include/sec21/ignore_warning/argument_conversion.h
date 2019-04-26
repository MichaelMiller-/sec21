
// MSVC: 'argument': conversion from 'U' to 'T', possible loss of data

//! \todo clang
#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wunused-variable"
#  if __has_warning("-Wnew-special-warning")
#   pragma clang diagnostic ignored "-Wnew-special-warning"
#  endif
#endif

//! \todo gcc

// https://stackoverflow.com/questions/4193476/is-using-pragma-warning-push-pop-the-right-way-to-temporarily-alter-warning-lev#18463996
// https://github.com/ruslo/leathers/wiki/List

#if defined(_MSC_VER)
#  pragma warning(disable: 4244)
#endif