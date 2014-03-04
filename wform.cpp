#include "wform.h"
#include <boost/format.hpp>
#include "wt_helpers.h"
#include <Wt/WFormWidget>
#include <Wt/WLabel>

using namespace Wt;
using namespace std;
namespace WtCommons {
  class WForm::Private {
    public:
      Private(WForm::Type type, int labelColumns) : type(type), labelColumns(labelColumns) {}
      const WForm::Type type;
      const int labelColumns;
  };

  WForm::WForm(Type type, int labelColumns, WContainerWidget *parent) : WContainerWidget(parent), d(new Private(type, labelColumns))
  {
    static map<Type, string> typeClass {
      {Default, ""},
      {Horizontal, "form-horizontal"},
      {Inline, "form-inline"},
    };
    addStyleClass(typeClass[type]);
    setAttributeValue("role", "form");
  }


  WForm *WForm::addButton(WWidget *w, bool isFormControl) {
    return add(w, string(), isFormControl);
  }

  WForm *WForm::add(WWidget *w, const string &labelKey, bool isFormControl) {
    string controlSize = (boost::format("col-sm-%d") % (12-d->labelColumns)).str();
    string labelSize = (boost::format("col-sm-%d") % (d->labelColumns)).str();
    string controlOffset = (boost::format("col-sm-offset-%d") % (d->labelColumns)).str();
    WContainerWidget *container = WW<WContainerWidget>().addCss("form-group");
    if(isFormControl)
      w->addStyleClass("form-control");

    if(!labelKey.empty()) {
      WLabel *label = WW<WLabel>(WString::tr(labelKey), container).addCss("control-label").addCss(labelSize);
      if(dynamic_cast<WFormWidget*>(w))
        label->setBuddy(dynamic_cast<WFormWidget*>(w));
    }
    container->addWidget(WW<WContainerWidget>().addCss(controlSize).addCss(labelKey.empty() ? controlOffset : "").add(w));
    addWidget(container);
    return this;
  }

} // namespace WtCommons
