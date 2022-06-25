use interoptopus::{function, Inventory, InventoryBuilder};

mod binder;
mod glue;
mod wrapper;

#[allow(dead_code)]
pub fn public_api() -> Inventory {
    InventoryBuilder::new()
        .register(function!(wrapper::setup_p2p_session))
        .register(function!(wrapper::setup_spectator_session))
        .register(function!(wrapper::setup_synctest_session))
        .register(function!(wrapper::start_p2p_session))
        .register(function!(wrapper::start_spectator_session))
        .register(function!(wrapper::start_synctest_session))
        .register(function!(wrapper::create_session))
        .register(function!(wrapper::cleanup_session))
        .register(function!(wrapper::poll_remote_clients))
        .register(function!(wrapper::add_player))
        .register(function!(wrapper::frames_ahead))
        .register(function!(wrapper::current_state))
        .register(function!(wrapper::handle_events))
        .register(function!(wrapper::handle_requests))
        .register(function!(wrapper::add_local_input))
        .inventory()
}

#[test]
fn write_bindings() {
    let _res1 = binder::bindings_c(public_api());
}
