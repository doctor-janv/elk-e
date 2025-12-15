#include "elke_core/input/InputProcessor.h"
#include "elke_core/input/YAMLInput.h"
#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

void unitTestInputParameters()
{
  auto& core = FrameworkCore::getInstance();
  auto& logger = core.getLogger();
  auto& input_processor = core.inputProcessor();

  YAMLInput input_parser(logger);

  const auto data_tree =
    input_parser.parseInputFile("unitTest_InputParametersBlock.yaml");

  logger.log() << "Parsed data: \n" << data_tree.toStringAsYAML("");

  const auto base_input_parameters =
    InputParametersBlock(/*name=       */ "TestBlock",
                         /*description=*/"No description");

  //============================================= Define lambda
  auto checking_function = [&](const InputParametersBlock& input_parameters,
                               const std::string& child_name)
  {
    const auto& check = data_tree.child(child_name);

    for (const auto& check_data : check.children())
    {
      input_processor.clearInputCheckErrors();

      logger.log() << "Checking " << check_data->name();

      input_processor.checkInputParameters(
        check_data->name(), input_parameters, *check_data);

      const auto& errors = input_processor.getInputCheckErrors();

      if (not errors.empty())
        for (const auto& error : errors)
          logger.error() << "-----\n" + error + "-----\n";
    } // for check_data
  };

  //============================================= Test 1
  {
    auto input_parameters = base_input_parameters;

    input_parameters.addOptionalParameter("string_par", "", "");

    const std::string check_name = "string_parameter_cross_check";

    checking_function(input_parameters, check_name);
  }

} // void unitTestInputParameters()

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestInputParameters);
