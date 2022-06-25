use interoptopus::Inventory;
use interoptopus::{Error, Interop};

pub fn bindings_c(library: Inventory) -> Result<(), Error> {
    use interoptopus_backend_c::{Config, Generator};

    Generator::new(
        Config {
            ifndef: "C_GGRS".to_string(),
            ..Config::default()
        },
        library,
    )
    .write_file("binds/c_ggrs.h")?;

    Ok(())
}
