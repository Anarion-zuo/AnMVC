rm -rf docs/*
gitbook build doc docs
git add docs doc
git commit -m'update book'
git push
