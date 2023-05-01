# Maintainer: Jordan Dehmel <jdehmel@outlook.com>

pkgname="jknit-git"
pkgver="0.0.8"
pkgrel="1"
pkgdesc=".jmd/.rmd to .tex compiler for computational writeups"
arch=("x86_64")
depends=("clang" "make")
optdepends=("octave" "python3" "texlive-most")
license=("GPLv3")
source=("git+https://github.com/jorbDehmel/jknit.git")
sha512sums=("SKIP")
provides=("jknit" "jknit-git")

package() {
	make -C jknit install
}
