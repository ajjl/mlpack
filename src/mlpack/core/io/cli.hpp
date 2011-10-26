/***
 * @file cli.hpp
 * @author Matthew Amidon
 *
 * This file implements the CLI subsystem which is intended to replace FX.
 * This can be used more or less regardless of context.  In the future,
 * it might be expanded to include file I/O.
 */
#ifndef MLPACK_CORE_IO_CLI_HPP
#define MLPACK_CORE_IO_CLI_HPP

#include <boost/program_options.hpp>
#include <boost/any.hpp>
#include <iostream>
#include <map>
#include <string>
#include <list>

#include "optionshierarchy.h"
#include "prefixedoutstream.h"
#include "nulloutstream.h"
#include "cli_deleter.hpp" // To make sure we can delete the singleton.

/**
 * Document an executable and set a default module.  Only one
 * instance of this macro should be present in your program!  Therefore, use it
 * in the main.cpp (or corresponding executable) in your program.
 *
 * The given default module will allow shorthand for options in that module.
 * For instance, if "bar" is given as DEF_MOD, then an argument "foo" with
 * parent "bar" could be specified on the command-line as "--foo=value" instead
 * of "--bar/foo=value".
 *
 * If you don't want to set a default module, pass an empty string ("") as
 * DEF_MOD.
 *
 * @see mlpack::CLI, PARAM_FLAG(), PARAM_INT(), PARAM_DOUBLE(), PARAM_STRING(),
 * PARAM_VECTOR(), PARAM_INT_REQ(), PARAM_DOUBLE_REQ(), PARAM_STRING_REQ(),
 * PARAM_VECTOR_REQ().
 *
 * @param NAME Short string representing the name of the program.
 * @param DESC Long string describing what the program does and possibly a
 *     simple usage example.  Newlines should not be used here; this is taken
 *     care of by CLI.
 * @param DEF_MOD A default module to use for parameters, mostly just to save
 *     excess typing.
 */
#define PROGRAM_INFO(NAME, DESC, DEF_MOD) static mlpack::io::ProgramDoc \
    io_programdoc_dummy_object = mlpack::io::ProgramDoc(NAME, DESC, DEF_MOD);

/**
 * Define a flag parameter.
 *
 * The parameter can then be specified on the command line with --PARENT/ID.
 * If PARENT is equal to DEF_MOD (which is set using the PROGRAM_INFO() macro),
 * the parameter can be specified with just --ID.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_FLAG(ID, DESC, PARENT) \
    PARAM_FLAG_INTERNAL(ID, DESC, PARENT);

/**
 * Define an integer parameter.
 *
 * The parameter can then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 * @param DEF Default value of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_INT(ID, DESC, PARENT, DEF) \
    PARAM(int, ID, DESC, PARENT, DEF, false)

/**
 * Define a floating-point parameter.  You should use PARAM_DOUBLE instead.
 *
 * The parameter can then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 * @param DEF Default value of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_FLOAT(ID, DESC, PARENT, DEF) \
    PARAM(float, ID, DESC, PARENT, DEF, false)

/**
 * Define a double parameter.
 *
 * The parameter can then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 * @param DEF Default value of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_DOUBLE(ID, DESC, PARENT, DEF) \
    PARAM(double, ID, DESC, PARENT, DEF, false)

/**
 * Define a string parameter.
 *
 * The parameter can then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 * @param DEF Default value of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_STRING(ID, DESC, PARENT, DEF) \
    PARAM(std::string, ID, DESC, PARENT, DEF, false)

/**
 * Define a vector parameter.
 *
 * The parameter can then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 * @param DEF Default value of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_VECTOR(T, ID, DESC, PARENT) \
    PARAM(std::vector<T>, ID, DESC, PARENT, std::vector<T>(), false)

// A required flag doesn't make sense and isn't given here.

/**
 * Define a required integer parameter.
 *
 * The parameter must then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_INT_REQ(ID, DESC, PARENT) PARAM(int, ID, DESC, PARENT, 0, true)

/**
 * Define a required floating-point parameter.  You should probably use a double
 * instead.
 *
 * The parameter must then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_FLOAT_REQ(ID, DESC, PARENT) PARAM(float, ID, DESC, PARENT, 0.0f, \
    true)

/**
 * Define a required double parameter.
 *
 * The parameter must then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_DOUBLE_REQ(ID, DESC, PARENT) PARAM(double, ID, DESC, PARENT, \
    0.0f, true)

/**
 * Define a required string parameter.
 *
 * The parameter must then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_STRING_REQ(ID, DESC, PARENT) PARAM(std::string, ID, DESC, \
    PARENT, "", true);

/**
 * Define a required vector parameter.
 *
 * The parameter must then be specified on the command line with
 * --PARENT/ID=value. If PARENT is equal to DEF_MOD (which is set using the
 * PROGRAM_INFO() macro), the parameter can be specified with just --ID=value.
 *
 * @param ID Name of the parameter.
 * @param DESC Quick description of the parameter (1-2 sentences).
 * @param PARENT Parent module of the parameter.
 *
 * @see mlpack::CLI, PROGRAM_INFO()
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros. However, not all
 * compilers have this support--most notably, gcc < 4.3. In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages.  See
 * http://mlpack.org/ticket/74 for more information.
 */
#define PARAM_VECTOR_REQ(T, ID, DESC, PARENT) PARAM(std::vector<T>, ID, DESC, \
    PARENT, std::vector<T>(), true);

/**
 * @cond
 * Don't document internal macros.
 */

// These are ugly, but necessary utility functions we must use to generate a
// unique identifier inside of the PARAM() module.
#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y
/** @endcond */

/**
 * Define an input parameter.  Don't use this function; use the other ones above
 * that call it.  Note that we are using the __LINE__ macro for naming these
 * actual parameters when __COUNTER__ does not exist, which is a bit of an ugly
 * hack... but this is the preprocessor, after all.  We don't have much choice
 * other than ugliness.
 *
 * @param T Type of the parameter.
 * @param ID Name of the parameter.
 * @param DESC Description of the parameter (1-2 sentences).
 * @param PARENT Name of parent module.
 * @param DEF Default value of the parameter.
 * @param REQ Whether or not parameter is required (boolean value).
 */
#ifdef __COUNTER__
  #define PARAM(T, ID, DESC, PARENT, DEF, REQ) static mlpack::io::Option<T> \
      JOIN(io_option_dummy_object_, __COUNTER__) \
      (false, DEF, ID, DESC, PARENT, REQ);

  /** @cond Don't document internal macros. */
  #define PARAM_FLAG_INTERNAL(ID, DESC, PARENT) static mlpack::io::Option<bool>\
  JOIN(__io_option_flag_object_, __COUNTER__) (ID, DESC, PARENT);
  /** @endcond */

  /**
   * Define a module.
   *
   * @param ID Name of the module.
   * @param DESC Description of the module (1-2 sentences).
   */
  #define PARAM_MODULE(ID, DESC) static mlpack::io::Option<int> \
      JOIN(io_option_module_dummy_object_, __COUNTER__) (true, 0, ID, DESC, \
      NULL);
#else
  // We have to do some really bizarre stuff since __COUNTER__ isn't defined.  I
  // don't think we can absolutely guarantee success, but it should be "good
  // enough".  We use the __LINE__ macro and the type of the parameter to try
  // and get a good guess at something unique.
  #define PARAM(T, ID, DESC, PARENT, DEF, REQ) static mlpack::io::Option<T> \
      JOIN(JOIN(io_option_dummy_object_, __LINE__), opt) (false, DEF, ID, \
      DESC, PARENT, REQ);

  /** @cond Don't document internal macros. */
  #define PARAM_FLAG_INTERNAL(ID, DESC, PARENT) static mlpack::io::Option<bool>\
      JOIN(__io_option_flag_object_, __LINE__) (ID, DESC, PARENT);
  /** @endcond */

  /**
   * Define a module.
   *
   * @param ID Name of the module.
   * @param DESC Description of the module (1-2 sentences).
   */
  #define PARAM_MODULE(ID, DESC) static mlpack::io::Option<int> \
      JOIN(JOIN(io_option_dummy_object_, __LINE__), mod) (true, 0, ID, DESC, \
      NULL);

#endif

/**
 * The TYPENAME macro is used internally to convert a type into a string.
 */
#define TYPENAME(x) (std::string(typeid(x).name()))

namespace po = boost::program_options;

namespace mlpack {

// Externally defined in option.hpp, this class holds information about the
// program being run.
namespace io {
class ProgramDoc;
};

/**
 * @brief Parses the command line for parameters and holds user-specified
 *     parameters.
 *
 * The CLI class is a subsystem by which parameters for machine learning methods
 * can be specified and accessed.  In conjunction with the macros PARAM_DOUBLE,
 * PARAM_INT, PARAM_STRING, PARAM_FLAG, and others, this class aims to make user
 * configurability of MLPACK methods very easy.
 *
 * @section addparam Adding parameters to a program
 *
 * Parameters held by the CLI class are hierarchical, meaning that each
 * parameter has a "parent" module.  On the command line, then, a double
 * parameter named "bar" which has a parent module named "foo" would be
 * specified to have a value of 5 like this:
 *
 * @code
 * $ ./executable --foo/bar=5
 * @endcode
 *
 * @note The = is optional; a space can also be used.
 *
 * So, each parameter that is specified must have a parent module; this helps
 * prevent name collisions.  Each module should be declared with some
 * documentation using the PARAM_MODULE(ID, DESC) macro.  Here is an example:
 *
 * @code
 * PARAM_MODULE("mvu", "Parameters for Maximum Variance Unfolding.");
 * @endcode
 *
 * A parameter is specified by using one of the following macros (this is not a
 * complete list; see core/io/cli.hpp):
 *
 *  - PARAM_FLAG(ID, DESC, PARENT)
 *  - PARAM_DOUBLE(ID, DESC, PARENT, DEF)
 *  - PARAM_INT(ID, DESC, PARENT, DEF)
 *  - PARAM_STRING(ID, DESC, PARENT, DEF)
 *
 * @param ID Name of the parameter.
 * @param DESC Short description of the parameter (one/two sentences).
 * @param PARENT Name of the parent module.
 * @param DEF Default value of the parameter.
 *
 * The flag (boolean) type automatically defaults to false; it is specified
 * merely as a flag on the command line (no '=true' is required).
 *
 * Here is an example of a few parameters being defined; this is for the L-BFGS
 * optimizer (mlpack::optimizers::L_BFGS):
 *
 * @code
 * PARAM_MODULE("lbfgs", "Options for the L-BFGS optimizer, which uses a "
 *    "back-tracing line search to determine the step size to take.");
 *
 * PARAM_DOUBLE("armijo_constant", "Controls the accuracy of the line search "
 *    "routine for determining the Armijo condition.", "lbfgs", 1e-4);
 * PARAM_DOUBLE("min_step", "The minimum step of the line search.", "lbfgs",
 *    1e-20);
 * PARAM_DOUBLE("max_step", "The maximum step of the line search.", "lbfgs",
 *    1e20);
 * PARAM_INT("max_line_search_trials", "The maximum number of trials for the "
 *    "line search.", "lbfgs", 50);
 * PARAM_DOUBLE("wolfe", "Parameter for detecting the Wolfe condition.",
 *    "lbfgs", 0.9);
 * PARAM_DOUBLE("min_gradient_norm", "Minimum gradient norm required to "
 *  "continue the optimization.", "lbfgs", 1e-10);
 * @endcode
 *
 * More documentation is available on the PARAM_*() macros in the documentation
 * for core/io/cli.hpp.
 *
 * @section programinfo Documenting the program itself
 *
 * In addition to allowing documentation for each individual parameter and
 * module, the PROGRAM_INFO() macro provides support for documenting the program
 * itself.  There should only be one instance of the PROGRAM_INFO() macro.
 * Below is an example:
 *
 * @code
 * PROGRAM_INFO("Maximum Variance Unfolding", "This program performs maximum "
 *    "variance unfolding on the given dataset, writing a lower-dimensional "
 *    "unfolded dataset to the given output file.", "mvu");
 * @endcode
 *
 * The last parameter, the default module (DEF_MOD), is discussed in detail in
 * the PROGRAM_INFO() documentation.
 *
 * @section parsecli Parsing the command line with CLI
 *
 * To have CLI parse the command line at the beginning of code execution, only a
 * call to ParseCommandLine() is necessary:
 *
 * @code
 * int main(int argc, char** argv) {
 *   CLI::ParseCommandLine(argc, argv);
 *
 *   ...
 * }
 * @endcode
 *
 * CLI provides --help and --info options which give nicely formatted
 * documentation of each option; the documentation is generated from the DESC
 * arguments in the PARAM_*() macros.
 *
 * @section getparam Getting/setting parameters with CLI
 *
 * When the parameters have been defined, the next important thing is how to
 * access and modify them.  For this, the HasParam() and GetParam() methods are
 * used.  For instance, the option "k" with parent "neighbor_search" could be
 * modified like this (it could also be merely accessed with the same call as an
 * r-value).
 *
 * @code
 * CLI::GetParam<index_t>("neighbor_search/k") = 50;
 * @endcode
 *
 * @note
 * Care is needed when defining options.  Because the PARAM_*() macros expand to
 * a global object definition (of type mlpack::io::Option) whose constructor
 * adds the parameter to the hierarchy, the parameters defined in any included
 * file will be added to the program--and the documentation for those options
 * will appear when --help is given. For this reason, mlpack/core.h does not
 * include more than the core components necessary to write MLPACK code.  Care
 * is required so that only the files which are absolutely necessary are
 * included, so as to avoid cluttering the documentation with irrelevant
 * options.
 *
 * @bug
 * The __COUNTER__ variable is used in most cases to guarantee a unique global
 * identifier for options declared using the PARAM_*() macros.  However, not all
 * compilers have this support--most notably, gcc < 4.3.  In that case, the
 * __LINE__ macro is used as an attempt to get a unique global identifier, but
 * collisions are still possible, and they produce bizarre error messages. See
 * http://mlpack.org/ticket/74 for more information.
 */
class CLI {
 public:
  /**
   * Adds a parameter to the hierarchy; use the PARAM_*() macros instead of this
   * (i.e. PARAM_INT()). Uses char* and not std::string since the vast majority
   * of use cases will be literal strings.
   *
   * @param identifier The name of the parameter.
   * @param description Short string description of the parameter.
   * @param parent Full pathname of a parent module, default is the root node
   *    ("").
   * @param required Indicates if parameter must be set on command line.
   */
  static void Add(const char* identifier,
                  const char* description,
                  const char* parent = NULL,
                  bool required = false);

  /**
   * Adds a parameter to the hierarchy; use the PARAM_*() macros instead of this
   * (i.e. PARAM_INT()). Uses char* and not std::string since the vast majority
   * of use cases will be literal strings.  If the argument requires a
   * parameter, you must specify a type.
   *
   * @param identifier The name of the parameter.
   * @param description Short string description of the parameter.
   * @param parent Full pathname of a parent module, default is root node.
   * @param required Indicates if parameter must be set on command line.
   */
  template<class T>
  static void Add(const char* identifier,
                  const char* description,
                  const char* parent,
                  bool required = false);

  /**
   * Adds a flag parameter to the hierarchy; use PARAM_FLAG() instead of this.
   *
   * @param identifier The name of the paramater.
   * @param description Short string description of the parameter.
   * @param parent Full pathname of the parent module; default is root node.
   */
  static void AddFlag(const char* identifier,
                      const char* description,
                      const char* parent);

  /**
   * See if the specified flag was found while parsing.
   *
   * @param identifier The name of the parameter in question.
   */
  static bool HasParam(const char* identifier);


  /**
   * Parses the parameters for 'help' and 'info'.
   * If found, will print out the appropriate information and kill the program.
   */
  static void DefaultMessages();

  /**
   * Takes all nodes at or below the specified module and returns a list of
   * their pathnames.
   *
   * @param folder The module to start gathering nodes at.
   *
   * @return A list of pathnames to everything at or below folder.
   */
  static std::vector<std::string> GetFolder(const char* folder);

  /**
   * Grab the value of type T found while parsing.  You can set the value using
   * this reference safely.
   *
   * @param identifier The name of the parameter in question.
   */
  template<typename T>
  static T& GetParam(const char* identifier);

  /**
   * Get the description of the specified node.
   *
   * @param identifier Name of the node in question.
   * @return Description of the node in question.
   */
  static std::string GetDescription(const char* identifier);

  /**
   * Searches for unqualified paramters; when one is found, the default module
   * is prepended onto it (if necessary).
   *
   * @param argc The number of parameters.
   * @param argv 2D array of the parameter strings.
   * @return Valid modified strings.
   */
  static std::vector<std::string> InsertDefaultModule(int argc, char** argv);

  /**
   * Parses the commandline for arguments.
   *
   * @param argc The number of arguments on the commandline.
   * @param argv The array of arguments as strings.
   */
  static void ParseCommandLine(int argc, char** argv);

  /**
   * Parses a stream for arguments.
   *
   * @param stream The stream to be parsed.
   */
  static void ParseStream(std::istream& stream);

  /**
   * Print out the current hierarchy.
   */
  static void Print();

  /**
   * Checks that all required parameters have been specified on the command
   * line.  If any have not been specified, an error message is printed and the
   * program is terminated.
   */
  static void RequiredOptions();

  /**
   * Cleans up input pathnames, rendering strings such as /foo/bar
   * and foo/bar/ equivalent inputs.
   *
   * @param str Input string.
   * @return Sanitized string.
   */
  static std::string SanitizeString(const char* str);

  /**
   * Initializes a timer, available like a normal value specified on
   * the command line.  Timers are of type timeval.
   *
   * @param timerName The name of the timer in question.
   */
  static void StartTimer(const char* timerName);

  /**
   * Halts the timer, and replaces its value with the amount of time elapsed
   * since the timer was started.
   *
   * @param timerName The name of the timer in question.
   */
  static void StopTimer(const char* timerName);

  /**
   * Parses the values given on the command line, overriding any default values.
   */
  static void UpdateGmap();

  /**
   * Registers a ProgramDoc object, which contains documentation about the
   * program.  If this method has been called before (that is, if two
   * ProgramDocs are instantiated in the program), a fatal error will occur.
   *
   * @param doc Pointer to the ProgramDoc object.
   */
  static void RegisterProgramDoc(io::ProgramDoc* doc);

  /**
   * Destroy the CLI object.  This resets the pointer to the singleton, so in
   * case someone tries to access it after destruction, a new one will be made
   * (the program will not fail).
   */
  static void Destroy();

  /**
   * Destructor.
   */
  ~CLI();

 private:
  //! The documentation and names of options.
  po::options_description desc;

  //! Stores a relative index of path names.
  io::OptionsHierarchy hierarchy;

  //! Values of the options given by user.
  po::variables_map vmap;

  //! Pathnames of required options.
  std::list<std::string> requiredOptions;

  //! Map of global values; stored here instead of in OptionsHierarchy for ease
  //! of implementation.
  std::map<std::string, boost::any> globalValues;

  //! The singleton itself.
  static CLI* singleton;

  //! True, if CLI was used to parse command line options.
  bool did_parse;

 public:
  //! Pointer to the ProgramDoc object.
  io::ProgramDoc *doc;

 private:
  /**
   * Retrieve the singleton.
   *
   * Not exposed to the outside, so as to spare users some ungainly
   * x.GetSingleton().foo() syntax.
   *
   * In this case, the singleton is used to store data for the static methods,
   * as there is no point in defining static methods only to have users call
   * private instance methods
   *
   * @return The singleton instance for use in the static methods.
   */
  static CLI& GetSingleton();

  /**
   * Properly formats strings such that there aren't too few or too many '/'s.
   *
   * @param id The name of the parameter, eg bar in foo/bar.
   * @param parent The full name of the parameter's parent,
   *   eg foo/bar in foo/bar/buzz.
   * @param tname String identifier of the parameter's type.
   * @param description String description of the parameter.
   */
  std::string ManageHierarchy(const char* id,
                              const char* parent,
                              std::string& tname,
                              const char* desc = "");

  /**
   * Add a parameter to the hierarchy.  We assume the string has already been
   * sanity-checked.
   *
   * @param path Full pathname of the parameter (parent/parameter).
   * @param tname String identifier of the parameter's type (TYPENAME(T)).
   * @param desc String description of the parameter (optional).
   */
  void AddToHierarchy(std::string& path, std::string& tname,
                      const char* desc = "");

#ifdef _WIN32
  /**
   * Converts a FILETIME structure to an equivalent timeval structure.
   * Only necessary on windows platforms.
   * @param tv Valid timeval structure.
   */
  void FileTimeToTimeVal(timeval* tv);
#endif

  /**
   * Make the constructor private, to preclude unauthorized instances.
   */
  CLI();

  /**
   * Initialize desc with a particular name.
   *
   * @param optionsName Name of the module, as far as boost is concerned.
   */
  CLI(std::string& optionsName);

  //! Private copy constructor; we don't want copies floating around.
  CLI(const CLI& other);
};

}; // namespace mlpack

// Include the actual definitions of templated methods
#include "cli_impl.hpp"

#endif