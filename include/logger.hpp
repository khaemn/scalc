#pragma once

#include <iostream>

class Logger : public std::ostream
{
public:
  static Logger &instance()
  {
    static Logger instance{};
    return instance;
  }

  template <typename T>
  friend Logger &operator<<(Logger &myo, const T &v);

  inline bool enabled() const
  {
    return enabled_;
  }

  inline void setEnabled(bool enabled)
  {
    enabled_ = enabled;
  }

private:
  explicit Logger()
    : std::ostream(std::cout.rdbuf())
  {}

  bool enabled_ = false;
};

template <typename T>
Logger &operator<<(Logger &logger, const T &v)
{
  if (logger.enabled())
  {
    static_cast<std::ostream &>(logger) << v;
    return logger;
  }
  return logger;
}
