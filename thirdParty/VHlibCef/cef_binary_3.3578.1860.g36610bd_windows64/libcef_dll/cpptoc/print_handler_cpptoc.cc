// Copyright (c) 2018 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=4a537ed250bfd093733b1c8caee531565162d368$
//

#include "libcef_dll/cpptoc/print_handler_cpptoc.h"
#include "libcef_dll/ctocpp/browser_ctocpp.h"
#include "libcef_dll/ctocpp/print_dialog_callback_ctocpp.h"
#include "libcef_dll/ctocpp/print_job_callback_ctocpp.h"
#include "libcef_dll/ctocpp/print_settings_ctocpp.h"

namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK
print_handler_on_print_start(struct _cef_print_handler_t* self,
                             cef_browser_t* browser) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser)
    return;

  // Execute
  CefPrintHandlerCppToC::Get(self)->OnPrintStart(
      CefBrowserCToCpp::Wrap(browser));
}

void CEF_CALLBACK
print_handler_on_print_settings(struct _cef_print_handler_t* self,
                                cef_browser_t* browser,
                                struct _cef_print_settings_t* settings,
                                int get_defaults) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser)
    return;
  // Verify param: settings; type: refptr_diff
  DCHECK(settings);
  if (!settings)
    return;

  // Execute
  CefPrintHandlerCppToC::Get(self)->OnPrintSettings(
      CefBrowserCToCpp::Wrap(browser), CefPrintSettingsCToCpp::Wrap(settings),
      get_defaults ? true : false);
}

int CEF_CALLBACK
print_handler_on_print_dialog(struct _cef_print_handler_t* self,
                              cef_browser_t* browser,
                              int has_selection,
                              cef_print_dialog_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser)
    return 0;
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback)
    return 0;

  // Execute
  bool _retval = CefPrintHandlerCppToC::Get(self)->OnPrintDialog(
      CefBrowserCToCpp::Wrap(browser), has_selection ? true : false,
      CefPrintDialogCallbackCToCpp::Wrap(callback));

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK
print_handler_on_print_job(struct _cef_print_handler_t* self,
                           cef_browser_t* browser,
                           const cef_string_t* document_name,
                           const cef_string_t* pdf_file_path,
                           cef_print_job_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser)
    return 0;
  // Verify param: document_name; type: string_byref_const
  DCHECK(document_name);
  if (!document_name)
    return 0;
  // Verify param: pdf_file_path; type: string_byref_const
  DCHECK(pdf_file_path);
  if (!pdf_file_path)
    return 0;
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback)
    return 0;

  // Execute
  bool _retval = CefPrintHandlerCppToC::Get(self)->OnPrintJob(
      CefBrowserCToCpp::Wrap(browser), CefString(document_name),
      CefString(pdf_file_path), CefPrintJobCallbackCToCpp::Wrap(callback));

  // Return type: bool
  return _retval;
}

void CEF_CALLBACK
print_handler_on_print_reset(struct _cef_print_handler_t* self,
                             cef_browser_t* browser) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: browser; type: refptr_diff
  DCHECK(browser);
  if (!browser)
    return;

  // Execute
  CefPrintHandlerCppToC::Get(self)->OnPrintReset(
      CefBrowserCToCpp::Wrap(browser));
}

cef_size_t CEF_CALLBACK
print_handler_get_pdf_paper_size(struct _cef_print_handler_t* self,
                                 int device_units_per_inch) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return CefSize();

  // Execute
  cef_size_t _retval =
      CefPrintHandlerCppToC::Get(self)->GetPdfPaperSize(device_units_per_inch);

  // Return type: simple
  return _retval;
}

}  // namespace

// CONSTRUCTOR - Do not edit by hand.

CefPrintHandlerCppToC::CefPrintHandlerCppToC() {
  GetStruct()->on_print_start = print_handler_on_print_start;
  GetStruct()->on_print_settings = print_handler_on_print_settings;
  GetStruct()->on_print_dialog = print_handler_on_print_dialog;
  GetStruct()->on_print_job = print_handler_on_print_job;
  GetStruct()->on_print_reset = print_handler_on_print_reset;
  GetStruct()->get_pdf_paper_size = print_handler_get_pdf_paper_size;
}

template <>
CefRefPtr<CefPrintHandler> CefCppToCRefCounted<
    CefPrintHandlerCppToC,
    CefPrintHandler,
    cef_print_handler_t>::UnwrapDerived(CefWrapperType type,
                                        cef_print_handler_t* s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template <>
base::AtomicRefCount CefCppToCRefCounted<CefPrintHandlerCppToC,
                                         CefPrintHandler,
                                         cef_print_handler_t>::DebugObjCt
    ATOMIC_DECLARATION;
#endif

template <>
CefWrapperType CefCppToCRefCounted<CefPrintHandlerCppToC,
                                   CefPrintHandler,
                                   cef_print_handler_t>::kWrapperType =
    WT_PRINT_HANDLER;
