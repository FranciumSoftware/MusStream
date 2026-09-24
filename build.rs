extern crate winres;
fn main() {
    slint_build::compile("ui/mainwindow.slint").unwrap();

    if cfg!(target_os = "windows") {
        let mut res = winres::WindowsResource::new();
        res.set_icon("icons/128x128@2x.ico");
        res.compile().unwrap();
    }
}
