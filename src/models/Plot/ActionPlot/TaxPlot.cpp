#include "models/Plot/ActionPlot/TaxPlot.hpp"
#include <iostream>

TaxPlot::TaxPlot(std::string name, std::string code, TaxType taxType, int flatAmount, int percentage)
    : ActionPlot(name, code),
      taxType(taxType),
      flatAmount(flatAmount),
      percentage(percentage) {}

std::string TaxPlot::getType() const {
    return (taxType == TaxType::PPH) ? "Petak Pajak Penghasilan" : "Petak Pajak Barang Mewah";
}

TaxType TaxPlot::getTaxType() const {
    return taxType;
}

int TaxPlot::getFlatAmount() const {
    return flatAmount;
}

int TaxPlot::getPercentage() const {
    return percentage;
}

void TaxPlot::startEvent() {
    if (taxType == TaxType::PPH) {
        std::cout << "Kamu mendarat di Pajak Penghasilan (PPH)!" << std::endl;
        std::cout << "Pilih opsi pembayaran pajak:" << std::endl;
        std::cout << "  1. Bayar flat M" << flatAmount << std::endl;
        std::cout << "  2. Bayar " << percentage << "% dari total kekayaan" << std::endl;
        std::cout << "(Pilih sebelum menghitung kekayaan!)" << std::endl;

    } else {
        std::cout << "Kamu mendarat di Pajak Barang Mewah (PBM)!" << std::endl;
        std::cout << "Pajak sebesar M" << flatAmount << " langsung dipotong." << std::endl;
    }
}

    // TODO: DEDUCTION NOT IMPLEMENTED - Sarannya nambahGameManager::handleTax()