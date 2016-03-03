/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSurface.h"
#include "SkImage.h"
#include "SkData.h"
#include "SkCanvas.h"
#include "SkColor.h"
#include "SkPaint.h"
#include "SkTypeface.h"

#include "SkCornerPathEffect.h"

#include "GrContext.h"
#include "GrTexture.h"

#include <assert.h>
#include <string>

class sk_paint_t : public SkRefCnt, public SkPaint {
public:
    sk_paint_t() { }
    explicit sk_paint_t(const sk_paint_t& paint) : SkPaint(paint) { }
};

typedef SkBitmap sk_bitmap_t;
typedef SkColorTable sk_color_table_t;
typedef SkPathEffect sk_path_effect_t;
typedef SkSurface sk_surface_t;
typedef SkImage sk_image_t;
typedef SkData sk_data_t;
typedef SkPath sk_path_t;
typedef SkTypeface sk_typeface_t;
typedef SkRegion sk_region_t;
typedef SkStrokeRec sk_stroke_rec_t;

typedef SkMatrix sk_matrix_t;

typedef GrRenderTarget sk_render_target_t;
typedef GrRenderTarget gr_render_target_t;
typedef GrContext gr_context_t;

typedef SkTypeface::Style sk_typeface_style_t;
typedef SkPath::Verb sk_path_verb_t;
typedef SkPath::Iter sk_path_iter_t;

#ifndef SKIA_IMPLEMENTATION
#define SKIA_IMPLEMENTATION 1
#endif  // SKIA_IMPLEMENTATION

#include "../include/skia.h"

extern "C" {
  /*static sk_error_t cache_management_to_skia(const sk_cache_management_t budget,
    SkSurface::Budget& bud) {
    switch(budget) {
    case sk_unbudgeted:
    bud = SkSurface::kNo_Budgeted;
    break;
    case sk_budgeted:
    bud = SkSurface::kYes_Budgeted;
    break;
    default:
    return sk_bad_arg_error;
    }
    return sk_no_error;
    }*/
  static sk_error_t color_type_to_skia(const sk_color_type_t ct,
                                       SkColorType& out) {
    switch(ct) {
    case sk_unknown_color_type:
      out = kUnknown_SkColorType;
      break;
    case sk_alpha_8_color_type:
      out = kAlpha_8_SkColorType;
      break;
    case sk_rgb_565_color_type:
      out = kRGB_565_SkColorType;
      break;
    case sk_argb_4444_color_type:
      out = kARGB_4444_SkColorType;
      break;
    case sk_rgba_8888_color_type:
      out = kRGBA_8888_SkColorType;
      break;
    case sk_bgra_8888_color_type:
      out = kBGRA_8888_SkColorType;
      break;
    case sk_index_8_color_type:
      out = kIndex_8_SkColorType;
      break;
      /*case sk_gray_8_color_type:
        out = kGray_8_SkColorType;
        break;*/
    default:
      return sk_bad_arg_error;
    }

    return sk_no_error;
  }
  static sk_error_t color_type_from_skia(const SkColorType& ct,
                                         sk_color_type_t& out) {
    switch(ct) {
    case kUnknown_SkColorType:
      out = sk_unknown_color_type;
      break;
    case kAlpha_8_SkColorType:
      out = sk_alpha_8_color_type;
      break;
    case kRGB_565_SkColorType:
      out = sk_rgb_565_color_type;
      break;
    case kARGB_4444_SkColorType:
      out = sk_argb_4444_color_type;
      break;
    case kRGBA_8888_SkColorType:
      out = sk_rgba_8888_color_type;
      break;
    case kBGRA_8888_SkColorType:
      out = sk_bgra_8888_color_type;
      break;
    case kIndex_8_SkColorType :
      out = sk_index_8_color_type;
      break;
      /*case sk_gray_8_color_type:
        out = kGray_8_SkColorType;
        break;*/
    default:
      out = sk_unknown_color_type;
      return sk_bad_arg_error;
    }

    return sk_no_error;
  }
  static sk_error_t alpha_type_to_skia(const sk_alpha_type_t at,
                                       SkAlphaType& out) {
    switch(at) {
    case sk_unknown_alpha_type:
      //out = kUnknown_SkAlphaType;
      out = kIgnore_SkAlphaType;
      break;
    case sk_opaque_alpha_type:
      out = kOpaque_SkAlphaType;
      break;
    case sk_premul_alpha_type:
      out = kPremul_SkAlphaType;
      break;
    case sk_unpremul_alpha_type:
      out = kUnpremul_SkAlphaType;
      break;
    default:
      return sk_bad_arg_error;
    }

    return sk_no_error;
  }
  static sk_error_t alpha_type_from_skia(const SkAlphaType& at,
                                         sk_alpha_type_t& out) {
    switch(at) {
    case kIgnore_SkAlphaType:
      //out = kUnknown_SkAlphaType;
      out = sk_unknown_alpha_type;
      break;
    case kOpaque_SkAlphaType:
      out = sk_opaque_alpha_type;
      break;
    case kPremul_SkAlphaType:
      out = sk_premul_alpha_type;
      break;
    case kUnpremul_SkAlphaType:
      out = sk_unpremul_alpha_type;
      break;
    default:
      return sk_bad_arg_error;
    }

    return sk_no_error;
  }
  /*static sk_error_t color_profile_to_skia(const sk_color_profile_t cp,
    SkColorProfileType& out) {
    switch(cp) {
    case sk_linear_color_profile_type:
    out = kLinear_SkColorProfileType;
    break;
    case sk_srgb_color_profile_type:
    out = kSRGB_SkColorProfileType;
    break;
    default:
    return sk_bad_arg_error;
    }

    return sk_no_error;
    }*/
  /*static sk_error_t pixel_geometry_to_skia(const sk_pixel_geometry_t pg,
    SkPixelGeometry& out) {
    switch(pg) {
    case sk_unknown_pixel_geometry:
    out = kUnknown_SkPixelGeometry;
    break;
    case sk_rgb_h_pixel_geometry:
    out = kRGB_H_SkPixelGeometry;
    break;
    case sk_bgr_h_pixel_geometry:
    out = kBGR_H_SkPixelGeometry;
    break;
    case sk_rgb_v_pixel_geometry:
    out = kRGB_V_SkPixelGeometry;
    break;
    case sk_bgr_v_pixel_geometry:
    out = kBGR_V_SkPixelGeometry;
    break;
    default:
    return sk_bad_arg_error;
    }

    return sk_no_error;
    }*/
  static sk_error_t image_info_to_skia(const sk_image_info_t img_info,
                                       SkImageInfo& out) {
    sk_error_t err;

    SkColorType ct;
    if(err = color_type_to_skia(img_info.color_type, ct)) {
      return err;
    }

    SkAlphaType at;
    if(err = alpha_type_to_skia(img_info.alpha_type, at)) {
      return err;
    }

    /*SkColorProfileType cp;
      if(err = color_profile_to_skia(img_info.color_profile, cp)) {
      return err;
      }*/

    out = SkImageInfo::Make(img_info.width, img_info.height,
                            ct, at);
    return sk_no_error;
  }
  static sk_error_t image_info_from_skia(const SkImageInfo& info,
                                         sk_image_info_t& out) {
    out.width = info.width();
    out.height = info.height();

    sk_error_t err;
    if(err = color_type_from_skia(info.colorType(), out.color_type)) {
      return err;
    } else if(err = alpha_type_from_skia(info.alphaType(), out.alpha_type)) {
      return err;
    }

    return sk_no_error;
  }

  static SkRect to_rect(sk_rect_t rect) {
    return SkRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom);
  }
  static sk_rect_t rect_from_skia(const SkRect& r) {
    sk_rect_t b;
    b.top = r.top();
    b.left = r.left();
    b.right= r.right();
    b.bottom = r.bottom();
    return b;
  }

  sk_image_info_t sk_default_image_info() {
    sk_image_info_t o;
    o.width = 0;
    o.height = 0;
    o.color_type = sk_unknown_color_type;
    o.alpha_type = sk_unknown_alpha_type;
    o.color_profile = sk_linear_color_profile_type;
    return o;
  }

  void sk_path_effect_ref(sk_path_effect_t* effect) {
    effect->ref();
  }
  void sk_path_effect_unref(sk_path_effect_t* effect) {
    effect->unref();
  }
  void sk_color_table_ref(sk_color_table_t* tbl) {
    tbl->ref();
  }
  void sk_color_table_unref(sk_color_table_t* tbl) {
    tbl->unref();
  }

  sk_error_t sk_surface_ref(sk_surface_t* sk_surface) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->ref();
    return sk_no_error;
  }

  sk_error_t sk_surface_unref(sk_surface_t* sk_surface) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->unref();
    return sk_no_error;
  }

  sk_error_t sk_image_ref(sk_image_t* sk_image) {
    if (!sk_image) {
      return sk_null_pointer_error;
    }
    sk_image->ref();
    return sk_no_error;

  }

  sk_error_t  sk_image_unref(sk_image_t* sk_image) {
    if (!sk_image) {
      return sk_null_pointer_error;
    }
    sk_image->unref();
    return sk_no_error;
  }

  sk_error_t  sk_data_ref(sk_data_t* sk_data) {
    if (!sk_data) {
      return sk_null_pointer_error;
    }
    sk_data->ref();
    return sk_no_error;
  }

  sk_error_t sk_data_unref(sk_data_t* sk_data) {
    if (!sk_data) {
      return sk_null_pointer_error;
    }
    sk_data->unref();
    return sk_no_error;
  }

  sk_error_t sk_paint_ref(sk_paint_t* sk_paint) {
    if (!sk_paint) {
      return sk_null_pointer_error;
    }
    sk_paint->ref();
    return sk_no_error;
  }

  sk_error_t sk_paint_unref(sk_paint_t* sk_paint) {
    if (!sk_paint) {
      return sk_null_pointer_error;
    }
    sk_paint->unref();
    return sk_no_error;
  }

  void sk_typeface_ref(sk_typeface_t* tf) {
    tf->ref();
  }
  void sk_typeface_unref(sk_typeface_t* tf) {
    tf->unref();
  }

  sk_surface_t* sk_new_raster_surface(sk_isize_t size) {
    SkImageInfo info = SkImageInfo::MakeN32Premul(size.width, size.height);
    return SkSurface::NewRaster(info);
  }

  sk_surface_t* sk_new_raster_direct_surface(const sk_image_info_t img_info, void* dest,
                                             const size_t row_bytes) {
    SkImageInfo image_info;
    if(image_info_to_skia(img_info, image_info)) {
      return NULL;
    }
    return SkSurface::NewRasterDirect(image_info, dest, row_bytes);
  }

  sk_surface_t* sk_new_render_target_direct_surface(
                                                    sk_render_target_t* sk_render_target) {
    return SkSurface::NewRenderTargetDirect(sk_render_target);
  }
  sk_surface_t* sk_new_render_target_surface(gr_context_t* ctxt,
                                             const sk_cache_management_t _budget,
                                             const sk_image_info_t img_info) {
    if(ctxt == NULL) { return NULL; }

    /*SkSurface::Budgeted bud;
      if(cache_management_to_skia(budget, bud)) {
      return nullptr;
      }*/

    SkImageInfo image_info;
    if(image_info_to_skia(img_info, image_info)) {
      return NULL;
    }

    return SkSurface::NewScratchRenderTarget(ctxt, image_info);
  }

  sk_image_t* sk_new_image_snapshot(sk_surface_t* sk_surface) {
    if (!sk_surface) {
      return NULL;
    }
    return sk_surface->newImageSnapshot();
  }
  GrBackendObject sk_image_get_gr_backing_handle(sk_image_t* img) {
    if(img == NULL) { return 0; }

    if(img->getTexture() == NULL) { return 0; }

    return img->getTexture()->getTextureHandle();
  }

  sk_data_t* sk_new_data(const void* data,
                         size_t size,
                         sk_length_release_procedure_t proc,
                         void* context) {
    if (!data) {
      return NULL;
    }
    return SkData::NewWithProc(data, size, proc, context);
  }

  sk_paint_t* sk_new_paint()  {
    return SkNEW(sk_paint_t);
  }

  sk_paint_t* sk_copy_paint(sk_paint_t* sk_paint)  {
    if (sk_paint) {
      return SkNEW_ARGS(sk_paint_t, (*sk_paint));
    }
    return SkNEW(sk_paint_t);
  }


  sk_data_t* sk_new_encoded_bmp(const sk_image_t* sk_image)  {
    return NULL;
  }

  sk_data_t* sk_new_encoded_gif(const sk_image_t* sk_image)  {
    if (sk_image) {
      return sk_image->encode(SkImageEncoder::kGIF_Type, 100);
    }
    return NULL;
  }

  sk_data_t* sk_new_encoded_jpeg(const sk_image_t* sk_image, int quality)  {
    if (sk_image) {
      return sk_image->encode(SkImageEncoder::kJPEG_Type, quality);
    }
    return NULL;
  }

  sk_data_t* sk_new_encoded_png(const sk_image_t* sk_image)  {
    if (sk_image) {
      return sk_image->encode(SkImageEncoder::kPNG_Type, 100);
    }
    return NULL;
  }

  sk_data_t* sk_new_encoded_webp(const sk_image_t* sk_image, int quality)  {
    if (sk_image) {
      return sk_image->encode(SkImageEncoder::kWEBP_Type, quality);
    }
    return NULL;
  }

  sk_error_t sk_image_peek_pixels(const sk_image_t* sk_image,
                                  sk_isize_t *sizeOut,
                                  sk_color_type_t* colorTypeOut,
                                  size_t* rowBytesOut,
                                  const void** pixelsOut) {
    if (!(sk_image && sizeOut && colorTypeOut && rowBytesOut && pixelsOut)) {
      return sk_null_pointer_error;
    }
    SkImageInfo info;
    size_t rowBytes;
    const void* pixels = sk_image->peekPixels(&info, &rowBytes);
    if ((info.colorType() == kN32_SkColorType)
        && (info.alphaType() == kPremul_SkAlphaType)) {
      *sizeOut = (sk_isize_t){info.width(), info.height()};
      *pixelsOut = pixels;
      *colorTypeOut = sk_native32_color_type;
      *rowBytesOut = rowBytes;
      return sk_no_error;
    }
    return sk_other_error;
  }


  sk_error_t sk_surface_get_size(const sk_surface_t* sk_surface,
                                 sk_isize_t* sizeOut) {
    if (!(sk_surface && sizeOut)) {
      return sk_null_pointer_error;
    }
    *sizeOut = (sk_isize_t){sk_surface->width(), sk_surface->height()};
    return sk_no_error;
  }
  sk_image_info_t sk_surface_get_image_info(const sk_surface_t* surface) {
    sk_image_info_t info;
    image_info_from_skia(const_cast<sk_surface_t*>(surface)->getCanvas()->imageInfo(), info);
    return info;
  }

  sk_error_t sk_image_get_size(const sk_image_t* sk_image, sk_isize_t* sizeOut) {
    if (!(sk_image && sizeOut)) {
      return sk_null_pointer_error;
    }
    *sizeOut = (sk_isize_t){sk_image->width(), sk_image->height()};
    return sk_no_error;
  }

  ////////////////////////////////////////////////////////////////////////////////

  sk_error_t sk_data_get_data(sk_data_t* sk_data, const void** data, size_t* size) {
    if (!sk_data) {
      return sk_null_pointer_error;
    }
    if (data) {
      *data = sk_data->data();
    }
    if (size) {
      *size = sk_data->size();
    }
    return sk_no_error;
  }

  // Typeface:

  sk_typeface_t* sk_typeface_create_from_name(const char* family_name, const size_t len,
                                              sk_typeface_style_t style) {
    std::string family_name_cstr(family_name, len);
    return SkTypeface::CreateFromName(family_name_cstr.c_str(), style);
  }
  sk_typeface_t* sk_typeface_create_from_typeface(const sk_typeface_t* family, sk_typeface_style_t style) {
    return SkTypeface::CreateFromTypeface(family, style);
  }
  sk_typeface_t* sk_typeface_create_from_path(const char* path, const size_t len) {
    std::string path_cstr(path, len);
    return SkTypeface::CreateFromFile(path_cstr.c_str());
  }

  // /Typeface

  // Effect

  sk_path_effect_t* sk_new_corner_path_effect(SkScalar radius) {
    return SkCornerPathEffect::Create(radius);
  }
  bool sk_path_effect_filter_path(const sk_path_effect_t* effect, sk_path_t* dest,
                                  const sk_path_t* src, sk_stroke_rec_t* stroke,
                                  const sk_rect_t* cull) {
    if(cull != NULL) {
      const SkRect skcull = to_rect(*cull);
      return effect->filterPath(dest, *src, stroke, &skcull);
    } else {
      return effect->filterPath(dest, *src, stroke, NULL);
    }
  }

  // /Effect

  // ColorTable

  sk_color_table_t* sk_new_color_table(const SkPMColor* colors, const size_t len,
                                       const sk_alpha_type_t alpha_type) {
    SkAlphaType skat;
    if(alpha_type_to_skia(alpha_type, skat)) {
      return NULL;
    } else {
      return SkNEW(SkColorTable(colors, (int) len, skat));
    }
  }

  // /ColorTable

  // Bitmap

  sk_bitmap_t* sk_new_empty_bitmap() {
    return SkNEW(sk_bitmap_t);
  }
  void sk_reset_bitmap(sk_bitmap_t* bm) {
    bm->reset();
  }
  void sk_delete_bitmap(sk_bitmap_t* bm) {
    delete bm;
  }

  bool sk_bitmap_draws_nothing(const sk_bitmap_t* bm) {
    return bm->drawsNothing();
  }

  bool sk_bitmap_get_immutable(const sk_bitmap_t* bm) {
    return bm->isImmutable();
  }
  void sk_bitmap_set_immutable(sk_bitmap_t* bm) {
    bm->setImmutable();
  }
  size_t sk_bitmap_row_bytes(const sk_bitmap_t* bm) {
    return bm->rowBytes();
  }
  sk_image_info_t sk_bitmap_get_image_info(const sk_bitmap_t* bm) {
    sk_image_info_t info = sk_default_image_info();
    image_info_from_skia(bm->info(), info);
    return info;
  }
  bool sk_bitmap_set_image_info(sk_bitmap_t* bm, const sk_image_info_t info,
                                const size_t row_bytes) {
    SkImageInfo sk_info;
    image_info_to_skia(info, sk_info);
    return bm->setInfo(sk_info, row_bytes);
  }
  void sk_bitmap_set_pixels(sk_bitmap_t* bm, void* pixels, sk_color_table_t* ctable) {
    bm->setPixels(pixels, ctable);
  }
  void* sk_bitmap_get_pixels(const sk_bitmap_t* bm) {
    return bm->getPixels();
  }

  // /Bitmap

  sk_error_t sk_paint_reset(sk_paint_t* sk_paint)  {
    if (!sk_paint) {
      return sk_null_pointer_error;
    }
    sk_paint->reset();
    return sk_no_error;
  }

  sk_color_t sk_paint_get_color(const sk_paint_t* sk_paint) {
    return sk_paint->getColor();
  }

  void sk_paint_set_color(sk_paint_t* sk_paint, sk_color_t color)  {
    sk_paint->setColor(color);
  }

  bool sk_paint_get_anti_alias(const sk_paint_t* paint) {
    return paint->isAntiAlias();
  }
  void sk_paint_set_anti_alias(sk_paint_t* paint, const bool antialias) {
    paint->setAntiAlias(antialias);
  }

  bool sk_paint_get_subpixel_text(const sk_paint_t* paint) {
    return paint->isSubpixelText();
  }
  void sk_paint_set_subpixel_text(sk_paint_t* paint, const bool subpixel) {
    paint->setSubpixelText(subpixel);
  }

  bool sk_paint_get_lcd_render_text(const sk_paint_t* paint) {
    return paint->isLCDRenderText();
  }
  void sk_paint_set_lcd_render_text(sk_paint_t* paint, const bool v) {
    paint->setLCDRenderText(v);
  }

  sk_typeface_t* sk_paint_get_typeface(const sk_paint_t* paint) {
    return paint->getTypeface();
  }
  void sk_paint_set_typeface(sk_paint_t* paint, sk_typeface_t* tf) {
    paint->setTypeface(tf);
  }

  SkScalar sk_paint_get_text_size(const sk_paint_t* paint) {
    return paint->getTextSize();
  }
  void sk_paint_set_text_size(sk_paint_t* paint, const SkScalar size) {
    paint->setTextSize(size);
  }
  SkScalar sk_paint_get_text_x_scale(const sk_paint_t* paint) {
    return paint->getTextScaleX();
  }
  void sk_paint_set_text_x_scale(sk_paint_t* paint, const SkScalar x) {
    paint->setTextScaleX(x);
  }
  SkPaint::Align sk_paint_get_text_align(const sk_paint_t* paint) {
    return paint->getTextAlign();
  }
  void sk_paint_set_text_align(sk_paint_t* paint, const SkPaint::Align align) {
    paint->setTextAlign(align);
  }

  SkPaint::TextEncoding sk_paint_get_text_encoding(const sk_paint_t* paint) {
    return paint->getTextEncoding();
  }
  void sk_paint_set_text_encoding(sk_paint_t* paint, const SkPaint::TextEncoding encoding) {
    paint->setTextEncoding(encoding);
  }
  SkPaint::Style sk_paint_get_style(const sk_paint_t* paint) {
    return paint->getStyle();
  }
  void sk_paint_set_style(sk_paint_t* paint, const SkPaint::Style s) {
    paint->setStyle(s);
  }
  sk_path_effect_t* sk_paint_get_path_effect(const sk_paint_t* paint) {
    return paint->getPathEffect();
  }
  void sk_paint_set_path_effect(sk_paint_t* paint, sk_path_effect_t* effect) {
    paint->setPathEffect(effect);
  }

  SkScalar sk_paint_measure_text(const sk_paint_t* paint, const void* text, const size_t length,
                                 sk_rect_t* bounds, SkScalar scale) {
    if(bounds != NULL) {
      SkRect b;
      SkScalar result = paint->measureText(text, length, &b, scale);
      *bounds = rect_from_skia(b);
      return result;
    } else {
      return paint->measureText(text, length, NULL, scale);
    }
  }

  // The following are canvas draw commands

  sk_error_t sk_flush(sk_surface_t* sk_surface) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->flush();
    return sk_no_error;
  }

  void sk_surface_discard(sk_surface_t* surface) {
    surface->getCanvas()->discard();
  }

  sk_error_t sk_save(sk_surface_t* sk_surface, int* saveCount) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    int count = sk_surface->getCanvas()->save();
    if (saveCount) {
      *saveCount = count;
    }
    return sk_no_error;
  }
  sk_error_t sk_restore(sk_surface_t* sk_surface) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->restore();
    return sk_no_error;
  }
  sk_error_t sk_restore_to_count(sk_surface_t* sk_surface, int saveCount) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->restoreToCount(saveCount);
    return sk_no_error;
  }

  sk_error_t sk_translate(sk_surface_t* sk_surface, float dx, float dy) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->translate(dx, dy);
    return sk_no_error;
  }
  sk_error_t sk_scale(sk_surface_t* sk_surface, float sx, float sy) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->scale(sx, sy);
    return sk_no_error;
  }
  sk_error_t sk_rotate(sk_surface_t* sk_surface, float degrees) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->rotate(degrees);
    return sk_no_error;
  }

  void sk_surface_clear(sk_surface_t* surface, const sk_color_t color) {
    surface->getCanvas()->clear(color);
  }

  int sk_surface_save_layer_alpha(sk_surface_t* surface, const sk_rect_t* bounds,
                                  const uint8_t alpha) {
    if(bounds != NULL) {
      SkRect b = to_rect(*bounds);
      return surface->getCanvas()->saveLayerAlpha(&b, alpha);
    } else {
      return surface->getCanvas()->saveLayerAlpha(NULL, alpha);
    }
  }
  sk_error_t sk_clip_rect(sk_surface_t* sk_surface, sk_rect_t rect) {
    if (!sk_surface) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->clipRect(to_rect(rect));
    return sk_no_error;
  }
  void sk_surface_draw_line(sk_surface_t* surface, const sk_paint_t* paint,
                            const sk_point_t start, const sk_point_t end) {
    surface->getCanvas()->drawLine(start.x, start.y,
                                   end.x, end.y,
                                   *paint);
  }
  sk_error_t sk_draw_paint(sk_surface_t* sk_surface, sk_paint_t* sk_paint) {
    if (!(sk_surface && sk_paint)) {
      return sk_null_pointer_error;
    }
    sk_surface->getCanvas()->drawPaint(*sk_paint);
    return sk_no_error;
  }

  sk_error_t sk_draw_points(sk_surface_t* sk_surface, const sk_paint_t* sk_paint,
                            sk_point_mode_t point_mode, const sk_point_t* points,
                            const size_t point_count) {
    if(sk_paint == NULL || sk_paint == NULL ||
       points == NULL || point_count == 0) {
      return sk_bad_arg_error;
    }
    SkCanvas::PointMode mode;
    switch(point_mode) {
    case sk_points_point_mode:
      mode = SkCanvas::kPoints_PointMode;
      break;
    case sk_lines_point_mode:
      mode = SkCanvas::kLines_PointMode;
      break;
    case sk_polygon_point_mode:
      mode = SkCanvas::kPolygon_PointMode;
      break;
    default:
      return sk_bad_arg_error;
    }

    sk_surface->getCanvas()->drawPoints(mode, point_count, reinterpret_cast<const SkPoint*>(points),
                                        *sk_paint);
    return sk_no_error;
  }
  sk_error_t sk_draw_path(sk_surface_t* sk_surface, const sk_paint_t* sk_paint,
                          const sk_path_t* path) {
    if(sk_surface == NULL || sk_paint == NULL ||
       path == NULL) {
      return sk_bad_arg_error;
    }

    sk_surface->getCanvas()->drawPath(*path, *sk_paint);
    return sk_no_error;
  }
  void sk_draw_bitmap_rect_to_rect(sk_surface_t* surface, const sk_paint_t* paint,
                                   const sk_bitmap_t* bitmap, const sk_rect_t* src,
                                   const sk_rect_t dest, const bool bleed) {
    SkCanvas::DrawBitmapRectFlags flags = SkCanvas::kNone_DrawBitmapRectFlag;
    if(bleed) {
      flags = SkCanvas::kBleed_DrawBitmapRectFlag;
    }

    if(src != NULL) {
      SkRect sksrc = to_rect(*src);
      surface->getCanvas()->drawBitmapRectToRect(*bitmap, &sksrc, to_rect(dest),
                                                 paint, flags);
    } else {
      surface->getCanvas()->drawBitmapRectToRect(*bitmap, NULL, to_rect(dest),
                                                 paint, flags);
    }
  }
  /*sk_error_t sk_draw_image_rect(sk_surface_t* sk_surface, const sk_paint_t* paint,
    const sk_rect_t* src, sk_rect_t dest,
    const sk_image_t* img, const sk_src_rect_constraint_t constraint) {
    if(sk_surface == NULL || paint == NULL || img == NULL) {
    return sk_bad_arg_error;
    }
    SkCanvas::SrcRectConstraint src_rect;
    switch(constraint) {
    case sk_strict_src_rect_constraint:
    src_rect = SkCanvas::kStrict_SrcRectConstraint;
    break;
    case sk_fast_src_rect_constraint:
    src_rect = SkCanvas::kFast_SrcRectConstraint;
    break;
    default:
    return sk_bad_arg_error;
    }

    if(src != NULL) {
    sk_surface->getCanvas()->drawImageRect(img, to_rect(*src), to_rect(dest),
    paint, src_rect);
    } else {
    sk_surface->getCanvas()->drawImageRect(img, nullptr, to_rect(dest),
    paint, src_rect);
    }
    return sk_no_error;
    }*/

  sk_error_t sk_draw_text(sk_surface_t* sk_surface, const sk_paint_t* sk_paint,
                          sk_point_t pos, const void* text, const size_t text_length) {
    if(sk_surface == NULL || sk_paint == NULL || text == NULL || text_length == 0) {
      return sk_bad_arg_error;
    }

    sk_surface->getCanvas()->drawText(text, text_length, pos.x, pos.y, *sk_paint);
    return sk_no_error;
  }

  ////////////////////////////////////////////////////////////////////////////////

  // sk_path_t

  sk_path_t* sk_new_path() {
    return SkNEW(SkPath);
  }
  sk_path_t* sk_clone_path(const sk_path_t* path) {
    if(path == NULL) {
      return NULL;
    } else {
      return SkNEW(SkPath(*path));
    }
  }
  void sk_del_path(sk_path_t* path) {
    delete path;
  }
  sk_error_t sk_path_reset(sk_path_t* path) {
    if(path == NULL) {
      return sk_bad_arg_error;
    } else {
      path->reset();
      return sk_no_error;
    }
  }
  sk_error_t sk_path_set_fill_type(sk_path_t* path, const sk_path_fill_type_t ft) {
    if(path == NULL) {
      return sk_bad_arg_error;
    }

    SkPath::FillType ft2;
    switch(ft) {
    case sk_winding_fill_type:
      ft2 = SkPath::kWinding_FillType;
      break;
    case sk_even_odd_fill_type:
      ft2 = SkPath::kEvenOdd_FillType;
      break;
    case sk_inverse_winding_fill_type:
      ft2 = SkPath::kInverseWinding_FillType;
      break;
    case sk_inverse_even_odd_fill_type:
      ft2 = SkPath::kInverseEvenOdd_FillType;
      break;
    default:
      return sk_bad_arg_error;
    }

    path->setFillType(ft2);
    return sk_no_error;
  }
  sk_path_fill_type_t sk_path_get_fill_type(const sk_path_t* path) {
    assert(path != NULL);

    const SkPath::FillType ft = path->getFillType();
    switch(ft) {
    case SkPath::kWinding_FillType: return sk_winding_fill_type;
    case SkPath::kEvenOdd_FillType: return sk_even_odd_fill_type;
    case SkPath::kInverseWinding_FillType: return sk_inverse_winding_fill_type;
    case SkPath::kInverseEvenOdd_FillType: return sk_inverse_even_odd_fill_type;
    default:
      assert(0 && "unknown fill type");
    }
  }
  void sk_path_move_to(sk_path_t* path, const sk_point_t point, const bool relative) {
    if(!relative) {
      path->moveTo(point.x, point.y);
    } else {
      path->rLineTo(point.x, point.y);
    }
  }
  sk_error_t sk_path_line_to(sk_path_t* path, const sk_point_t point, const bool relative) {
    if(path == NULL) { return sk_bad_arg_error; }

    if(!relative) {
      path->lineTo(point.x, point.y);
    } else {
      path->rLineTo(point.x, point.y);
    }

    return sk_no_error;
  }
  sk_error_t sk_path_quad_to(sk_path_t* path, const sk_point_t p1, const sk_point_t p2,
                             const bool relative) {
    if(path == NULL) { return sk_bad_arg_error; }

    if(!relative) {
      path->quadTo(p1.x, p1.y, p2.x, p2.y);
    } else {
      path->rQuadTo(p1.x, p1.y, p2.x, p2.y);
    }

    return sk_no_error;
  }
  sk_error_t sk_path_cubic_to(sk_path_t* path, const sk_point_t p1, const sk_point_t p2,
                              const sk_point_t p3, const bool relative) {
    if(path == NULL) { return sk_bad_arg_error; }

    if(!relative) {
      path->cubicTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    } else {
      path->rCubicTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    }

    return sk_no_error;
  }
  void sk_path_close(sk_path_t* path) {
    path->close();
  }
  int sk_path_count_points(const sk_path_t* path) {
    assert(path != NULL);

    return path->countPoints();
  }
  sk_point_t sk_path_get_point(const sk_path_t* path, const int index) {
    assert(path != NULL);

    const SkPoint pt = path->getPoint(index);
    return {
      pt.x(), pt.y(),
        };
  }

  void sk_path_init_iter(sk_path_iter_t* iter, const sk_path_t* path, const bool force_close) {
    new (iter) SkPath::Iter(*path, force_close);
  }
  void sk_path_deinit_iter(sk_path_iter_t* iter) {
    iter->~Iter();
  }
  sk_path_verb_t sk_path_iter_next(sk_path_iter_t* iter, sk_point_t pts[4], const bool consume_degerate) {
    SkPoint skpts[4] = {
      { pts[0].x, pts[0].y },
      { pts[1].x, pts[1].y },
      { pts[2].x, pts[2].y },
      { pts[3].x, pts[3].y },
    };

    return iter->next(skpts, consume_degerate);
  }

  // SkPaint?

  sk_color_t sk_color_from_argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b)  {
    return SkColorSetARGBMacro(a, r, g, b);
  }

  uint8_t sk_color_get_a(sk_color_t color)  {
    return SkColorGetA(color);
  }

  uint8_t sk_color_get_r(sk_color_t color)  {
    return SkColorGetR(color);
  }

  uint8_t sk_color_get_g(sk_color_t color)  {
    return SkColorGetG(color);
  }

  uint8_t sk_color_get_b(sk_color_t color)  {
    return SkColorGetB(color);
  }
  sk_color_t sk_color_premul(sk_color_t in) {
    return SkPreMultiplyColor(in);
  }

  void gr_context_flush(gr_context_t* gr, gr_context_flush_bits_t bits) {
    gr->flush(bits);
  }

}
