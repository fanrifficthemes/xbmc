/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/*!
 * @file StereoscopicsManager.cpp
 * @brief This class acts as container for stereoscopic related functions
 */

#pragma once

#include <stdlib.h>
#include "settings/lib/ISettingCallback.h"
#include "guilib/IMsgTargetCallback.h"
#include "rendering/RenderSystem.h"

class CAction;
class CDataCacheCore;
class CSettings;

enum STEREOSCOPIC_PLAYBACK_MODE
{
  STEREOSCOPIC_PLAYBACK_MODE_ASK,
  STEREOSCOPIC_PLAYBACK_MODE_PREFERRED,
  STEREOSCOPIC_PLAYBACK_MODE_MONO,

  STEREOSCOPIC_PLAYBACK_MODE_IGNORE = 100,
};

class CStereoscopicsManager : public ISettingCallback,
                              public IMsgTargetCallback
{
public:
  CStereoscopicsManager(CSettings &settings,
                        CDataCacheCore &dataCacheCore);
  ~CStereoscopicsManager(void) override;

  void Initialize(void);

  RENDER_STEREO_MODE GetStereoMode(void) const;

  void SetStereoModeByUser(const RENDER_STEREO_MODE &mode);
  void SetStereoMode(const RENDER_STEREO_MODE &mode);

  RENDER_STEREO_MODE GetNextSupportedStereoMode(const RENDER_STEREO_MODE &currentMode, int step = 1) const;
  std::string DetectStereoModeByString(const std::string &needle) const;
  RENDER_STEREO_MODE GetStereoModeByUserChoice(const std::string &heading = "") const;
  RENDER_STEREO_MODE GetStereoModeOfPlayingVideo(void) const;
  std::string GetLabelForStereoMode(const RENDER_STEREO_MODE &mode) const;
  RENDER_STEREO_MODE GetPreferredPlaybackMode(void) const;
  static int ConvertVideoToGuiStereoMode(const std::string &mode);
  /**
   * @brief will convert a string representation into a GUI stereo mode
   * @param mode The string to convert
   * @return -1 if not found, otherwise the according int of the RENDER_STEREO_MODE enum
   */
  static int ConvertStringToGuiStereoMode(const std::string &mode);
  static const char* ConvertGuiStereoModeToString(const RENDER_STEREO_MODE &mode);

  /**
   * @brief Converts a stereoscopics related action/command from Builtins and JsonRPC into the according cAction ID.
   * @param command The command/action
   * @param parameter The parameter of the command
   * @return The integer of the according cAction or -1 if not valid
   */
  static CAction ConvertActionCommandToAction(const std::string &command, const std::string &parameter);
  static std::string NormalizeStereoMode(const std::string &mode);

  void OnSettingChanged(std::shared_ptr<const CSetting> setting) override;
  void OnStreamChange();
  bool OnMessage(CGUIMessage &message) override;
  /*!
   * @brief Handle 3D specific cActions
   * @param action The action to process
   * @return True if action could be handled, false otherwise.
   */
  bool OnAction(const CAction &action);

private:
  void ApplyStereoMode(const RENDER_STEREO_MODE &mode, bool notify = true);
  void OnPlaybackStopped(void);
  std::string GetVideoStereoMode() const;
  bool IsVideoStereoscopic() const;

  // Construction parameters
  CSettings &m_settings;
  CDataCacheCore &m_dataCacheCore;

  // Stereoscopic parameters
  RENDER_STEREO_MODE m_stereoModeSetByUser;
  RENDER_STEREO_MODE m_lastStereoModeSetByUser;
};
