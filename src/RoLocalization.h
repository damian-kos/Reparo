#pragma once
#include <string>
#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

// Add this if pgettext is not available
#ifndef HAVE_PGETTEXT
#define HAVE_PGETTEXT 1
inline const char* pgettext(const char* context, const char* msgid) {
  size_t contextlen = strlen(context) + 1;
  size_t msgidlen = strlen(msgid) + 1;
  char* msg_ctxt_id = new char[contextlen + msgidlen];

  memcpy(msg_ctxt_id, context, contextlen - 1);
  msg_ctxt_id[contextlen - 1] = '\004'; // EOT
  memcpy(msg_ctxt_id + contextlen, msgid, msgidlen);

  const char* translation = gettext(msg_ctxt_id);

  if (translation == msg_ctxt_id) {
    translation = msgid;
  }

  delete[] msg_ctxt_id;
  return translation;
}
#endif
namespace Loc {
  void initLocalization();
  void renderUI();
}

class Localization {
public:
  static Localization& getInstance() {
    static Localization instance;
    return instance;
  }

  bool init(const std::string& domain, const std::string& localePath) {
#ifdef _WIN32
    // On Windows, set locale to UTF-8
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(CP_UTF8);
#else
    // On Unix-like systems, simply set the locale
    setlocale(LC_ALL, "");
#endif

    // Bind text domain for localization
    bindtextdomain(domain.c_str(), localePath.c_str());
    bind_textdomain_codeset(domain.c_str(), "UTF-8");
    textdomain(domain.c_str());

    currentDomain = domain;
    return true;
  }

  bool switchLanguage(const std::string& locale) {
#ifdef _WIN32
    // Use _putenv_s for safer environment variable handling on Windows
    _putenv_s("LANGUAGE", locale.c_str());
#else
    // Use setenv on Unix-like systems
    setenv("LANGUAGE", locale.c_str(), 1);
#endif
    return true;
  }

  std::string getCurrentLocale() const {
#ifdef _WIN32
    // Use _dupenv_s to safely get the environment variable on Windows
    char* buffer = nullptr;
    size_t bufferSize = 0;
    errno_t err = _dupenv_s(&buffer, &bufferSize, "LANGUAGE");
    if (err == 0 && buffer) {
      std::string locale(buffer);
      free(buffer);
      return locale;
    }
    else {
      return "eng";  // Default locale
    }
#else
    // On Unix-like systems, use getenv
    const char* lang = getenv("LANGUAGE");
    return lang ? lang : "eng";  // Default to "eng" if not set
#endif
  }

private:
  Localization() = default;
  std::string currentDomain;
};

// Convenience macro for translation
#define _(str) gettext(str)
// Macro for translation with context
#define C_(ctx, str) pgettext(ctx, str)
// Macro for plural forms
#define N_(singular, plural, n) ngettext(singular, plural, n)