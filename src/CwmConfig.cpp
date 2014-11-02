#include <CwmI.h>
#include <CConfig.h>

CwmConfig *
CwmConfig::
getInstance()
{
  static CwmConfig *instance;

  if (! instance)
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

std::string
CwmConfig::
getSectionValue(const std::string &path, const std::string &section, const std::string &fallback)
{
  std::string value = fallback;

  if (! config_->getValue(path, section, value))
    value = fallback;

  return value;
}

std::string
CwmConfig::
getValue(const std::string &path, const std::string &fallback)
{
  std::string value = fallback;

  if (! config_->getValue(path, "", value))
    value = fallback;

  return value;
}

int
CwmConfig::
getSectionValue(const std::string &path, const std::string &section, int fallback)
{
  int value = fallback;

  std::string str;

  if (! config_->getValue(path, section, str))
    return value;

  if (CStrUtil::isInteger(str))
    value = CStrUtil::toInteger(str);

  return value;
}

int
CwmConfig::
getValue(const std::string &path, int fallback)
{
  int value = fallback;

  std::string str;

  if (! config_->getValue(path, "", str))
    return value;

  if (CStrUtil::isInteger(str))
    value = CStrUtil::toInteger(str);

  return value;
}

void
CwmConfig::
setSectionValue(const std::string &path, const std::string &section, const std::string &value)
{
  config_->setValue(path, section, value);
}

void
CwmConfig::
setValue(const std::string &path, const std::string &value)
{
  config_->setValue(path, "", value);
}

void
CwmConfig::
getGroups(const std::string &path, StringVectorT &groupNames, StringVectorT &groupPaths)
{
  config_->getGroups(path, groupNames, groupPaths);
}

void
CwmConfig::
getSections(const std::string &path, StringVectorT &sections)
{
  config_->getSections(path, sections);
}
