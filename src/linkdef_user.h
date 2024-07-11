/**
 * @file   linkdef_user.h
 * @brief
 *
 * @date   Created:       2013-08-12 14:48:07
 *         Last Modified: Apr 16, 2015 04:19:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro, OTA Shinsuke All rights reserved
 */

#ifndef LINKDEF_USER_H
#define LINKDEF_USER_H

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// main
#pragma link C++ class art::TTemplateProcessor;
#pragma link C++ class art::TTemplateData + ;
// mwdc
#pragma link C++ class art::TMWDCTrackingProcessor_crib;

#endif // __CINT__

#endif // LINKDEF_USER_H
