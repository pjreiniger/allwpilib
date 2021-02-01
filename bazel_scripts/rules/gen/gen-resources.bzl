
def _generate_resource_impl(ctx):
    outputs = []

    for i, lbl in enumerate(ctx.attr.resource_files):

      lbl = lbl.replace("src/app/native/resources/", "")
      func_name = "GetResource_" + lbl.replace("-", "_").replace(".", "_")

      out = ctx.actions.declare_file("Generated" + str(i) + ".cpp")
      content = """
#include <stddef.h>
#include <wpi/StringRef.h>
extern "C" {
static const unsigned char contents[] = { ${data} };
const unsigned char* ${prefix}${func_name}(size_t* len) {
  *len = ${data_size};
  return contents;
}
}
namespace glass {
wpi::StringRef ${func_name}() {
  return wpi::StringRef(reinterpret_cast<const char*>(contents), ${data_size});
}
}

      """.replace("${data}", "0x00").replace("${data_size}", "1").replace("${func_name}",func_name).replace("${prefix}", "GLASS_")

      ctx.actions.write(
          output = out,
          content = content,
      )

      outputs.append(out)

    return [DefaultInfo(files = depset(outputs))]

generate_resources = rule(
    implementation = _generate_resource_impl,
    attrs = {
        "resource_files": attr.string_list(mandatory=True),
        # "template": attr.label(
        #     allow_single_file = True, # [".cc.tpl"],
        #     mandatory = True,
        # ),
    },
)
