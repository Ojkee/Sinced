from dataclasses import dataclass
from settings.enums import Task_Status



@dataclass()
class Task:
    id: int
    text: str
    date_added: str
    deadline: str | None = None
    status: Task_Status = Task_Status.ON_GOING

    def __repr__(self) -> str:
        return f"""{self.id} | "{self.text}"\nCreated: {self.date_added}\nDeadline: {self.deadline}"""


    def get_deadline_str(self) -> str:
        return "" if self.deadline is None else f"{self.deadline}"

    @property
    def Status(self) -> Task_Status:
        return self.status

    @Status.setter
    def Status(self, val: Task_Status) -> None:
        self.status = val
