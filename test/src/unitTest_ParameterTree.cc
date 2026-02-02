#include "elke_core/input/InputProcessor.h"
#include "elke_core/input/YAMLInput.h"
#include "elke_core/FrameworkCore.h"
#include "elke_core/parameters2/ParameterTree.h"

#define performTest(check_name, command)                                       \
  {                                                                            \
    auto params = ParameterTree("NoName", "Description");                      \
    command;                                                                   \
    checking_function(params, check_name, /*verbose=*/true);                   \
  }

namespace elke::unit_tests
{

void unitTestParameterTree()
{
  auto& core = FrameworkCore::getInstance();
  const auto& input_processor = core.inputProcessor();
  auto& logger = core.getLogger();

  YAMLInput input_parser(logger);

  const auto data_tree =
    input_parser.parseInputFile("unitTest_ParameterTree.yaml");

  logger.log() << "Parsed data: \n" << data_tree.toStringAsYAML("");

  // ####################################################### Lambda
  auto checking_function = [&](ParameterTree& input_tree,
                               const std::string& child_name,
                               const bool verbose = true)
  {
    const auto& check = data_tree.child(child_name);

    for (const auto& check_data_ptr : check.constChildren())
    {
      const auto& check_data = *check_data_ptr;
      StatusStrings status_strings;

      input_tree.checkAndAssignData(status_strings, check_data);

      const bool has_errors = not status_strings.m_errors.empty();

      const std::string status =
        has_errors ? "Error(s) produced." : "No error.";

      logger.log() << "Checking " << child_name << "/" << check_data.name()
                   << ". " << status;

      if (verbose and has_errors)
      {
        WarningsAndErrorsData warnings_and_errors_data;
        warnings_and_errors_data.m_errors.emplace_back(status_strings.m_errors);
        input_processor.postWarningsAndErrors(warnings_and_errors_data);
      }
    } // for check_data
    logger.log() << std::endl;
  };
  // #######################################################

  logger.log() << "UNIT_TEST_BEGIN";

  logger.log() << "====================================================";
  logger.log() << "Testing OPTIONAL SCALAR parameters";
  performTest("string_parameter_cross_check",
              params.addOptionalParameter("test_param", "", std::string{}););
  performTest("boolean_parameter_cross_check",
              params.addOptionalParameter("test_param", "", bool{}););
  performTest("integer_parameter_cross_check",
              params.addOptionalParameter("test_param", "", int{}););
  performTest("float_parameter_cross_check",
              params.addOptionalParameter("test_param", "", float{}););
  performTest("float_parameter_cross_check",
              params.addOptionalParameter("test_param", "", 2.0););

  logger.log() << "====================================================";
  logger.log() << "Testing REQUIRED SCALAR parameters WITH template parameters";
  performTest("string_parameter_cross_check",
              params.addRequiredParameter<std::string>("test_param", ""););
  performTest("boolean_parameter_cross_check",
              params.addRequiredParameter<bool>("test_param", ""););
  performTest("integer_parameter_cross_check",
              params.addRequiredParameter<int>("test_param", ""););
  performTest("float_parameter_cross_check",
              params.addRequiredParameter<float>("test_param", ""););

  logger.log() << "====================================================";
  logger.log() << "Testing OPTIONAL SEQUENCE parameters with vector";
  performTest(
    "array_parameter_cross_check",
    params.addOptionalParameter("test_param", "", std::vector<int>{}););

  logger.log() << "====================================================";
  logger.log()
    << "Testing OPTIONAL SEQUENCE parameters with vector and custom checks";
  {
    const std::string check_name = "array_parameter_special_checks";
    auto params = ParameterTree("NoName", "Description");
    auto& param =
      params.addOptionalParameter("test_param", "", std::vector<int>{});

    auto new_ptr = std::make_unique<param_checks::ArraySizeCheck>(3);
    param.addAdditionalInputCheck(std::move(new_ptr));
    param.addAdditionalInputCheck(
      {std::make_unique<param_checks::ArraySizeCheck>(3)});

    checking_function(params, check_name, /*verbose=*/true);
  }

  logger.log() << "====================================================";
  logger.log() << "Testing OPTIONAL SEQUENCE parameters with arbitrary values";
  {
    const std::string check_name = "array_parameter_of_arb_type";
    auto params = ParameterTree("NoName", "Description");
    auto& param =
      params.addOptionalParameter("test_param", "", GenericParameterArray());

    class CustomCheck : public param_checks::InputCheckCloneAble<CustomCheck>
    {
    public:
      bool performCheck(StatusStrings& status_strings,
                        const DataTree& data,
                        const std::string& prefix,
                        const std::string& name_or_id) const override
      {
        bool has_errors = false;

        if (data.numChildren() != 5)
        {
          status_strings.m_errors += prefix + "Bad error in CustomCheck\n";
          return false;
        }

        const auto data_gross_types = data.makeChildrenGrossTypesList();

        const auto reqr_gross_types = std::vector{DataGrossType::SCALAR,
                                                  DataGrossType::SCALAR,
                                                  DataGrossType::SCALAR,
                                                  DataGrossType::SCALAR,
                                                  DataGrossType::MAP};

        if (data_gross_types != reqr_gross_types)
        {
          std::stringstream error_message;
          const auto data_typenames = DataGrossTypeNames(data_gross_types);
          const auto reqr_typenames = DataGrossTypeNames(reqr_gross_types);
          error_message << prefix << "The entries for item " << name_or_id
                        << " are required to follow the gross-type sequence "
                        << reqr_typenames << " but the supplied types are "
                        << data_typenames << ".\n";
          status_strings.m_errors += error_message.str();
          has_errors = true;
        }

        return has_errors;
      }
    };

    param.addAdditionalInputCheck(
      {std::make_unique<param_checks::ArraySizeCheck>(5)},
      /*allow_subsequent_checks=*/false);
    param.addAdditionalInputCheck({std::make_unique<CustomCheck>()});

    checking_function(params, check_name, /*verbose=*/true);
  }

  logger.log() << "====================================================";
  logger.log() << "Testing OPTIONAL SEQUENCE parameters with arbitrary map";
  {
    const std::string check_name = "map_parameter";
    auto params = ParameterTree("NoName", "Description");

    params.addOptionalParameter("test_param", "", GenericParameterMap());

    checking_function(params, check_name, /*verbose=*/true);
  }

  logger.log() << "UNIT_TEST_END";

} // void unitTestInputParameters()

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestParameterTree);
