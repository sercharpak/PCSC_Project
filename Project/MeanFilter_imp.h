//
// Created by shernand on 12/4/17.
//

template<typename T> MeanFilter<T>::MeanFilter() : StandardFilter<T>()
{
    this->setLength(5);
    int pLength = this->length;
    std::vector<T> pMask =this->mask;
    pMask = std::vector<T>(pLength);
    std::for_each(pMask.begin(), pMask.end(),
                  [pLength](T &x) { x = (1.0/pLength); }); //Fills the mask
    this->setMask(pMask);
}
template<typename T> MeanFilter<T>::MeanFilter(int pLength) : StandardFilter<T>(pLength)
{
    this->length=pLength;
    std::vector<T> mask = std::vector<T>(pLength);
    std::for_each(mask.begin(), mask.end(),
                  [pLength](T &x) { x = (1.0/pLength); }); //Fills the mask
    this->setMask(mask);
}