#include "elke_core/input/InputProcessor.h"
#include "elke_core/input/YAMLInput.h"
#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

void unitTestInputParameters()
{
  auto& core = FrameworkCore::getInstance();
  const auto& input_processor = core.inputProcessor();
  auto& logger = core.getLogger();

  YAMLInput input_parser(logger);

  const auto data_tree =
    input_parser.parseInputFile("unitTest_InputParametersBlock.yaml");

  logger.log() << "Parsed data: \n" << data_tree.toStringAsYAML("");

  logger.log() << "UNIT_TEST_BEGIN";

  const auto base_input_parameters =
    InputParametersBlock(/*name=       */ "TestBlock",
                         /*description=*/"No description");

  //============================================= Define lambda
  auto checking_function = [&](const InputParametersBlock& input_parameters,
                               const std::string& child_name,
                               const bool verbose = true)
  {
    const auto& check = data_tree.child(child_name);

    for (const auto& check_data_ptr : check.children())
    {
      const auto& check_data = *check_data_ptr;
      WarningsAndErrorsData warnings_and_errors_data;

      input_parameters.checkInputDataValidity(
        check_data, warnings_and_errors_data /*in/out*/,
        /*nest_depth=*/0);

      std::string status = "No error.";
      if (not warnings_and_errors_data.m_errors.empty())
        status = "Error(s) produced.";

      logger.log() << "Checking " << child_name << "/" << check_data.name()
                   << ". " << status;
      if (verbose)
        input_processor.postWarningsAndErrors(warnings_and_errors_data);
    } // for check_data
    logger.log() << std::endl;
  };

  //============================================= Test 1 string param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("test_param", std::string(), "");

    const std::string check_name = "string_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 2 boolean param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("test_param", bool(), "");

    const std::string check_name = "boolean_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 3 integer param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("test_param", int(), "");

    const std::string check_name = "integer_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 4 float param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("test_param", float(), "");

    const std::string check_name = "float_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  // #################################################################
  logger.log()
    << "################################################# Array tests.";
  //============================================= Test 5 vector of string param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter(
      "test_param", std::vector<std::string>(), "");

    const std::string check_name = "array_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 6 vector of int param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("test_param", std::vector<int>(), "");

    const std::string check_name = "array_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 7 vector of float param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter(
      "test_param", std::vector<float>(), "");

    const std::string check_name = "array_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }
  //============================================= Test 8 vector of bool param
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter(
      "test_param", std::vector<bool>(), "");

    const std::string check_name = "array_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }

  // #################################################################
  logger.log() << "################################################# "
                  "Registered objects tests.";
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("illeparam", std::string(), "");
    input_parameters.addRequiredParameter<float>("scale", "");

    input_parameters.addOptionalParameter(
      "sub_object", RegisteredObjectProxy(), "");
    input_parameters.addOptionalParameter(
      "sub_object_array", std::vector<RegisteredObjectProxy>(), "");

    const std::string check_name = "reg_obj_parameter_cross_check";

    checking_function(input_parameters, check_name, true);
  }

  logger.log() << "UNIT_TEST_END";

} // void unitTestInputParameters()

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestInputParameters);
