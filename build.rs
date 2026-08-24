fn main() {
    slint_build::compile("ui/mainwindow.slint").unwrap();
    println!(r"cargo:rustc-link-search=native=C:\Program Files\VideoLAN\VLC");
}
