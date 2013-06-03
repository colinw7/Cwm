#include "CwmI.h"
#include "CConfig.h"

CwmConfig *
CwmConfig::
getInstance()
{
  static CwmConfig *instance;

  if (instance == NULL)
    instance = new CwmConfig();

  return instance;
}

CwmConfig::
CwmConfig()
{
  config_ = new CConfig("Cwm");
}

CwmConfig::
~CwmConfig()
{
  delete config_;
}

void
CwmConfig::
save()
{
  config_->save();
}

string
CwmConfig::
getSectionValue(const string &path, const string &section,
                const string &fallback)
{
  string value = fallback;

  if (! config_->getValue(path, section, value))
    value = fallback;

  return value;
}

string
CwmConfig::
getValue(const string &path, const string &fallback)
{
  string value = fallback;

  if (! config_->getValue(path, "", value))
    value = fallback;

  return value;
}

int
CwmConfig::
getSectionValue(const string &path, const string &section,
                int fallback)
{
  int value = fallback;

  string str;

  if (! config_->getValue(path, section, str))
    return value;

  if (CStrUtil::isInteger(str))
    value = CStrUtil::toInteger(str);

  return value;
}

int
CwmConfig::
getValue(const string &path, int fallback)
{
  int value = fallback;

  string str;

  if (! config_->getValue(path, "", str))
    return value;

  if (CStrUtil::isInteger(str))
    value = CStrUtil::toInteger(str);

  return value;
}

void
CwmConfig::
setSectionValue(const string &path, const string &section,
                const string &value)
{
  config_->setValue(path, section, value);
}

void
CwmConfig::
setValue(const string &path, const string &value)
{
  config_->setValue(path, "", value);
}

void
CwmConfig::
getGroups(const string &path, StringVectorT &groupNames,
          StringVectorT &groupPaths)
{
  config_->getGroups(path, groupNames, groupPaths);
}

void
CwmConfig::
getSections(const string &path, StringVectorT &sections)
{
  config_->getSections(path, sections);
}
