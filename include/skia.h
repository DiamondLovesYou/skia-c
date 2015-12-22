/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skia_DEFINED
#define skia_DEFINED

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>  /* int32_t */
#include <stdlib.h>  /* size_t */

#if !defined(SK_CPU_BENDIAN) && !defined(SK_CPU_LENDIAN)
    #if defined(__sparc) || defined(__sparc__) || \
      defined(_POWER) || defined(__powerpc__) || \
      defined(__ppc__) || defined(__hppa) || \
      defined(__PPC__) || defined(__PPC64__) || \
      defined(_MIPSEB) || defined(__ARMEB__) || \
      defined(__s390__) || \
      (defined(__sh__) && defined(__BIG_ENDIAN__)) || \
      (defined(__ia64) && defined(__BIG_ENDIAN__))
        #define SK_CPU_BENDIAN
    #else
        #define SK_CPU_LENDIAN
    #endif
#endif

/**
 *  We check to see if the SHIFT value has already been defined.
 *  if not, we define it ourself to some default values. We default to OpenGL
 *  order (in memory: r,g,b,a)
 */
#ifndef SK_A32_SHIFT
#  ifdef SK_CPU_BENDIAN
#    define SK_R32_SHIFT    24
#    define SK_G32_SHIFT    16
#    define SK_B32_SHIFT    8
#    define SK_A32_SHIFT    0
#  else
#    define SK_R32_SHIFT    0
#    define SK_G32_SHIFT    8
#    define SK_B32_SHIFT    16
#    define SK_A32_SHIFT    24
#  endif
#endif

/**
 * SkColor has well defined shift values, but SkPMColor is configurable. This
 * macro is a convenience that returns true if the shift values are equal while
 * ignoring the machine's endianness.
 */
#define SK_COLOR_MATCHES_PMCOLOR_BYTE_ORDER \
    (SK_A32_SHIFT == 24 && SK_R32_SHIFT == 16 && SK_G32_SHIFT == 8 && SK_B32_SHIFT == 0)

/**
 * SK_PMCOLOR_BYTE_ORDER can be used to query the byte order of SkPMColor at compile time. The
 * relationship between the byte order and shift values depends on machine endianness. If the shift
 * order is R=0, G=8, B=16, A=24 then ((char*)&pmcolor)[0] will produce the R channel on a little
 * endian machine and the A channel on a big endian machine. Thus, given those shifts values,
 * SK_PMCOLOR_BYTE_ORDER(R,G,B,A) will be true on a little endian machine and
 * SK_PMCOLOR_BYTE_ORDER(A,B,G,R) will be true on a big endian machine.
 */
#ifdef SK_CPU_BENDIAN
#  define SK_PMCOLOR_BYTE_ORDER(C0, C1, C2, C3)     \
        (SK_ ## C3 ## 32_SHIFT == 0  &&             \
         SK_ ## C2 ## 32_SHIFT == 8  &&             \
         SK_ ## C1 ## 32_SHIFT == 16 &&             \
         SK_ ## C0 ## 32_SHIFT == 24)
#else
#  define SK_PMCOLOR_BYTE_ORDER(C0, C1, C2, C3)     \
        (SK_ ## C0 ## 32_SHIFT == 0  &&             \
         SK_ ## C1 ## 32_SHIFT == 8  &&             \
         SK_ ## C2 ## 32_SHIFT == 16 &&             \
         SK_ ## C3 ## 32_SHIFT == 24)
#endif

#ifndef SKIA_IMPLEMENTATION
  /* the following are opaque types. */
  typedef struct sk_surface_struct sk_surface_t;
  typedef struct sk_image_struct sk_image_t;
  typedef struct sk_data_struct sk_data_t;
  typedef struct sk_render_target_struct sk_render_target_t;
  typedef struct sk_path_struct sk_path_t;
  typedef struct sk_paint_struct sk_paint_t;

  typedef struct gr_context_struct gr_context_t;

#define SK_API
#endif  /* SKIA_IMPLEMENTATION */

  /* Simple structs. */
  typedef struct { float left, top, right, bottom; } sk_rect_t;
  typedef struct { int32_t left, top, right, bottom; } sk_irect_t;
  typedef struct { float width, height; } sk_size_t;
  typedef struct { int32_t width, height; } sk_isize_t;
  typedef struct { float x, y; } sk_point_t;

  typedef void (*sk_release_procedure_t)(void* address, void* context);
  typedef void (*sk_length_release_procedure_t)(const void* address,
                                                size_t length,
                                                void* context);

  typedef uint32_t sk_color_t;

  typedef enum {
    sk_unknown_color_type = 0,
    sk_alpha_8_color_type = 1,
    sk_rgb_565_color_type = 2,
    sk_argb_4444_color_type = 3,
    sk_rgba_8888_color_type = 4,
    sk_bgra_8888_color_type = 5,
    sk_index_8_color_type = 6,
    sk_gray_8_color_type = 7,

#if SK_PMCOLOR_BYTE_ORDER(B,G,R,A)
    sk_native32_color_type = sk_bgra_8888_color_type,
#elif SK_PMCOLOR_BYTE_ORDER(R,G,B,A)
    sk_native32_color_type = sk_rgba_8888_color_type,
#else
# error "SK_*32_SHIFT values must correspond to BGRA or RGBA byte order"
#endif
  } sk_color_type_t;

  typedef enum {
    sk_unknown_alpha_type = 0,
    sk_opaque_alpha_type = 1,
    sk_premul_alpha_type = 2,
    sk_unpremul_alpha_type = 3,
  } sk_alpha_type_t;

  typedef enum {
    sk_linear_color_profile_type = 0,
    sk_srgb_color_profile_type = 1,
  } sk_color_profile_t;

  typedef enum {
    sk_no_error = 0,
    sk_other_error = 1,
    sk_null_pointer_error = 2,
    sk_bad_arg_error = 2,
  } sk_error_t;

  typedef enum {
    sk_points_point_mode = 0,
    sk_lines_point_mode = 1,
    sk_polygon_point_mode = 2,
  } sk_point_mode_t;
  typedef enum {
    sk_winding_fill_type = 0,
    sk_even_odd_fill_type = 1,
    sk_inverse_winding_fill_type = 2,
    sk_inverse_even_odd_fill_type = 3,
  } sk_path_fill_type_t;
  typedef enum {
    sk_strict_src_rect_constraint = 0,
    sk_fast_src_rect_constraint = 1,
  } sk_src_rect_constraint_t;
  typedef enum {
    sk_unbudgeted = 0,
    sk_budgeted = 1,
  } sk_cache_management_t;


  /*typedef enum {
    sk_unknown_pixel_geometry = 0,
    sk_rgb_h_pixel_geometry = 1,
    sk_bgr_h_pixel_geometry = 2,
    sk_rgb_v_pixel_geometry = 3,
    sk_bgr_v_pixel_geometry = 4,
    } sk_pixel_geometry_t;

    typedef struct {
    bool allow_aa;
    bool allow_dither;
    sk_pixel_geometry_t geometry;
    } sk_surface_props_t; */

  typedef struct {
    int width, height;
    sk_color_type_t color_type;
    sk_alpha_type_t alpha_type;
    sk_color_profile_t color_profile;
  } sk_image_info_t;

  SK_API sk_image_info_t sk_default_image_info();

  SK_API sk_error_t sk_surface_ref(sk_surface_t*);
  SK_API sk_error_t sk_surface_unref(sk_surface_t*);

  SK_API sk_error_t sk_image_ref(sk_image_t*);
  SK_API sk_error_t sk_image_unref(sk_image_t*);

  SK_API sk_error_t sk_data_ref(sk_data_t*);
  SK_API sk_error_t sk_data_unref(sk_data_t*);

  SK_API sk_error_t sk_paint_ref(sk_paint_t*);
  SK_API sk_error_t sk_paint_unref(sk_paint_t*);

  /*
    In general, any function that returns a pointer should have the
    corresponding TYPE_unref() function called to release memory.  If
    TYPE_ref() is called N times, TYPE_unref() should be called N+1
    times.
  */

  /* return NULL on error */
  SK_API sk_surface_t* sk_new_raster_surface(sk_isize_t size);

  SK_API sk_surface_t* sk_new_raster_direct_surface(sk_isize_t size,
                                                    sk_color_type_t colorType,
                                                    void* pixelAddress,
                                                    size_t rowBytes,
                                                    sk_release_procedure_t proc,
                                                    void* context);

  SK_API sk_surface_t* sk_new_render_target_direct_surface(sk_render_target_t*);
  SK_API sk_surface_t* sk_new_render_target_surface(gr_context_t* ctxt,
                                                    const sk_cache_management_t budget,
                                                    const sk_image_info_t img_info);
  SK_API sk_image_t* sk_new_image_snapshot(sk_surface_t*);

  SK_API sk_data_t* sk_new_data(const void* data,
                                size_t size,
                                sk_length_release_procedure_t proc,
                                void* context);

  SK_API sk_paint_t* sk_new_paint();
  SK_API sk_paint_t* sk_new_paint_copy(sk_paint_t*);

  /* These functions will fail (and return NULL) if the apropriate */
  /* libraries aren't linked to libskia. */
  SK_API sk_data_t* sk_new_encoded_bmp(const sk_image_t*);
  SK_API sk_data_t* sk_new_encoded_gif(const sk_image_t*);
  SK_API sk_data_t* sk_new_encoded_jpeg(const sk_image_t*, int quality);
  SK_API sk_data_t* sk_new_encoded_png(const sk_image_t*);
  SK_API sk_data_t* sk_new_encoded_webp(const sk_image_t*, int quality);

  SK_API sk_error_t sk_image_peek_pixels(const sk_image_t*,
                                         sk_isize_t *sizeOut,
                                         sk_color_type_t* colorTypeOut,
                                         size_t* rowBytesOut,
                                         const void** pixelsOut);

  SK_API sk_error_t sk_surface_get_size(const sk_surface_t*, sk_isize_t* sizeOut);

  SK_API sk_error_t sk_image_get_size(const sk_image_t*, sk_isize_t* sizeOut);

  SK_API sk_error_t sk_data_get_data(sk_data_t*, const void**, size_t*);

  /* skpaint */

  SK_API sk_error_t sk_paint_reset(sk_paint_t*);
  SK_API sk_color_t sk_paint_get_color(const sk_paint_t*);
  SK_API void sk_paint_set_color(sk_paint_t*, sk_color_t);

  /* TODO:  all other paint gettrs and setters. */

  /* The following are canvas draw commands  */

  SK_API sk_error_t sk_flush(sk_surface_t* sk_surface);

  SK_API sk_error_t sk_save(sk_surface_t* sk_surface, int* saveCount);
  SK_API sk_error_t sk_restore(sk_surface_t* sk_surface);
  SK_API sk_error_t sk_restore_to_count(sk_surface_t* sk_surface, int saveCount);

  SK_API sk_error_t sk_translate(sk_surface_t* sk_surface, float dx, float dy);
  SK_API sk_error_t sk_scale(sk_surface_t* sk_surface, float sx, float sy);
  SK_API sk_error_t sk_rotate(sk_surface_t* sk_surface, float degrees);

  SK_API sk_error_t sk_clip_rect(sk_surface_t* sk_surface, sk_rect_t rect);
  SK_API sk_error_t sk_draw_paint(sk_surface_t* sk_surface, sk_paint_t* sk_paint);
  SK_API sk_error_t sk_draw_points(sk_surface_t* sk_surface, const sk_paint_t* sk_paint,
                                   sk_point_mode_t point_mode, const sk_point_t* points,
                                   const size_t point_count);
  SK_API sk_error_t sk_draw_path(sk_surface_t* sk_surface, const sk_paint_t* sk_paint,
                                 const sk_path_t* path);
  /*SK_API sk_error_t sk_draw_image_rect(sk_surface_t* sk_surface, const sk_paint_t* paint,
                                       const sk_rect_t* src, sk_rect_t dest,
                                       const sk_image_t* img, const sk_src_rect_constraint_t constraint);*/
  SK_API sk_error_t sk_draw_text(sk_surface_t* sk_surface, const sk_point_t* sk_paint,
                                 sk_point_t pos, const void* text, const size_t text_length);

  /* sk_path_t */
  SK_API sk_path_t* sk_new_path();
  SK_API sk_path_t* sk_clone_path(const sk_path_t* path);
  SK_API void sk_del_path(sk_path_t* path);
  SK_API sk_error_t sk_path_reset(sk_path_t* path);
  SK_API sk_error_t sk_path_set_fill_type(sk_path_t* path, const sk_path_fill_type_t ft);
  SK_API sk_path_fill_type_t sk_path_get_fill_type(const sk_path_t* path);
  SK_API sk_error_t sk_path_line_to(sk_path_t* path, const sk_point_t point, const bool relative);
  SK_API sk_error_t sk_path_quad_to(sk_path_t* path, const sk_point_t p1, const sk_point_t p2,
                                    const bool relative);
  SK_API sk_error_t sk_path_cubic_to(sk_path_t* path, const sk_point_t p1, const sk_point_t p2,
                                     const sk_point_t p3, const bool relative);
  SK_API void sk_path_close(sk_path_t* path);
  SK_API int sk_path_count_points(const sk_path_t* path);
  SK_API sk_point_t sk_path_get_point(const sk_path_t* path, const int index);


  /* SkPaint? */

  SK_API sk_color_t sk_color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
  SK_API uint8_t sk_color_get_a(sk_color_t color);
  SK_API uint8_t sk_color_get_r(sk_color_t color);
  SK_API uint8_t sk_color_get_g(sk_color_t color);
  SK_API uint8_t sk_color_get_b(sk_color_t color);

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* skia_DEFINED */
