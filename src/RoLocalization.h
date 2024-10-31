#pragma once
#include <string>
#include <libintl.h>
#include <locale.h>
#include <libpq-fe.h>
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
    // On Windows, we need to set the locale using native Windows API
    // to properly handle UTF-8
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(CP_UTF8);
#else
    // On Unix-like systems, this is simpler
    setlocale(LC_ALL, "");
#endif

    bindtextdomain(domain.c_str(), localePath.c_str());
    bind_textdomain_codeset(domain.c_str(), "UTF-8");
    textdomain(domain.c_str());

    currentDomain = domain;
    return true;
  }

  bool switchLanguage(const std::string& locale) {
#ifdef _WIN32
    _putenv_s("LANGUAGE", locale.c_str());
#else
    setenv("LANGUAGE", locale.c_str(), 1);
#endif
    return true;
  }

  std::string getCurrentLocale() const {
    const char* lang = getenv("LANGUAGE");
    return lang ? lang : "eng";
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