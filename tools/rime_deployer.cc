//
// Copyleft 2012 RIME Developers
// License: GPLv3
//
// 2012-07-07 GONG Chen <chen.sst@gmail.com>
//
#include <iostream>
#include <string>
#include <rime/config.h>
#include <rime/deployer.h>
#include <rime/service.h>
#include <rime/lever/deployment_tasks.h>

int add_schema(int count, char* schemas[]) {
  rime::Config config;
  if (!config.LoadFromFile("default.custom.yaml")) {
    LOG(INFO) << "creating new file 'default.custom.yaml'.";
  }
  rime::ConfigMapEntryRef schema_list(config["patch"]["schema_list"]);
  for (int i = 0; i < count; ++i) {
    if (!schemas[i])
      return 1;
    std::string new_schema_id(schemas[i]);
    bool already_there = false;
    for (size_t j = 0; j < schema_list.size(); ++j) {
      if (!schema_list[j].HasKey("schema"))
        continue;
      const std::string schema_id(schema_list[j]["schema"].ToString());
      if (schema_id == new_schema_id) {
        already_there = true;
        break;
      }
    }
    if (already_there) continue;
    schema_list[schema_list.size()]["schema"] = new_schema_id;
    LOG(INFO) << "added schema: " << new_schema_id;
  }
  if (!config.SaveToFile("default.custom.yaml")) {
    LOG(ERROR) << "failed to save schema list.";
    return 1;
  }
  return 0;
}

int set_active_schema(const std::string& schema_id) {
  rime::Config config;
  if (!config.LoadFromFile("user.yaml")) {
    LOG(INFO) << "creating new file 'user.yaml'.";
  }
  config["var"]["previously_selected_schema"] = schema_id;
  if (!config.SaveToFile("user.yaml")) {
    LOG(ERROR) << "failed to set active schema: " << schema_id;
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  rime::SetupLogging("rime.tools");

  std::string option;
  std::string arg1, arg2;
  if (argc >= 2) option = argv[1];
  if (argc >= 3) arg1 = argv[2];
  if (argc >= 4) arg2 = argv[3];
  if (argc == 1) {
    std::cout << "options:" << std::endl
              << "\t--build [dest_dir [source_dir]]" << std::endl
              << "\t--add-schema schema_id [...]" << std::endl
              << "\t--set-active-schema schema_id" << std::endl
        ;
    return 0;
  }
  rime::Deployer& deployer(rime::Service::instance().deployer());
  if (argc >= 2 && argc <= 4 && option == "--build") {
    if (!arg1.empty()) {
      deployer.user_data_dir = arg1;
      if (!arg2.empty()) {
        deployer.shared_data_dir = arg2;
      }
      else {
        deployer.shared_data_dir = arg1;
      }
    }
    rime::WorkspaceUpdate update;
    return update.Run(&deployer) ? 0 : 1;
  }
  if (argc >= 3 && option == "--add-schema") {
    return add_schema(argc - 2, argv + 2);
  }
  if (argc == 3 && option == "--set-active-schema") {
    return set_active_schema(arg1);
  }
  std::cerr << "invalid arguments." << std::endl;
  return 1;
}
