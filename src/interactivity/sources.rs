use crate::sources;
use crate::{MainWindow, Source};
use slint::{ComponentHandle, ModelRc, VecModel};

/**
 * Shows the list of all saved sources in the ui
 */
pub fn show_sources(ui: &MainWindow) {
    let weak = ui.as_weak();
    let sources = match sources::get_sources() {
        Ok(sources) => sources,
        Err(error) => {
            eprintln!("Erreur lors du chargement des sources : {error}");
            return;
        }
    };
    let slint_sources: Vec<Source> = sources
        .into_iter()
        .map(|s| Source {
            id: s.id,
            name: s.name.into(),
            url: s.url.into(),
        })
        .collect();
    if let Some(ui) = weak.upgrade() {
        ui.set_sources(ModelRc::new(VecModel::from(slint_sources)));
    }
}
