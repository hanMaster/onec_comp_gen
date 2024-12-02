import useStore from "../store";
import FormOfMethods, { useFormOfMethods } from "./FormOfMethods";
import { ActionIcon, Badge, Box, Flex, Table, Tooltip } from "@mantine/core";
import IconAdd from "./IconAdd";
import IconTrash from "./IconTrash";
import IconEdit from "./IconEdit";

interface Props {
  isFunction: boolean;
}

export default function FunctionPanel({ isFunction }: Props) {
  const methods = useStore((state) => state.methods);
  const deleteMethod = useStore((state) => state.deleteMethod);

  const formOfMethods = useFormOfMethods();

  function addMethod() {
    formOfMethods.setUUID("");
    formOfMethods.toggleOpen();
  }

  function EditMethod(uuid: string) {
    formOfMethods.setUUID(uuid);
    formOfMethods.toggleOpen();
  }

  function DeleteMethod(uuid: string): void {
    deleteMethod(uuid);
  }

  return (
    <>
      <Box>
        {formOfMethods.open && (
          <FormOfMethods {...formOfMethods} isFunction={isFunction} />
        )}
        <Flex ml={5}>
          <ActionIcon onClick={addMethod} variant="subtle">
            <IconAdd />
          </ActionIcon>
        </Flex>
        <Table highlightOnHover withColumnBorders>
          <Table.Thead>
            <Table.Tr>
              <Table.Th>Имя</Table.Th>
              <Table.Th>Имя (eng)</Table.Th>
              <Table.Th>Параметры</Table.Th>
              {isFunction && <Table.Th>Тип возврата</Table.Th>}
              <Table.Th>Действия</Table.Th>
            </Table.Tr>
          </Table.Thead>
          <Table.Tbody>
            {methods
              .filter((item) => item.hasReturn == isFunction)
              .map((item) => (
                <Table.Tr
                  key={item.uuid}
                  onDoubleClick={() => EditMethod(item.uuid)}
                >
                  <Table.Td>{item.name}</Table.Td>
                  <Table.Td>{item.nameEng}</Table.Td>
                  <Table.Td>
                    <Flex gap={3}>
                      {item.callRustMethod && (
                        <Tooltip label="Установлен флаг вызор rust">
                          <Badge color="blue" size="xs" ml={3}>
                            rust
                          </Badge>
                        </Tooltip>
                      )}

                      {item.params.map((par) => (
                        <Badge
                          color="gray"
                          size="xs"
                          key={par.uuid}
                        >{`${par.name}:${par.type}`}</Badge>
                      ))}
                    </Flex>
                  </Table.Td>
                  {isFunction && <Table.Td>{item.ReturnType}</Table.Td>}

                  <Table.Td>
                    <Flex gap={10}>
                      <Tooltip label="Удалить строку">
                        <ActionIcon
                          variant="transparent"
                          color="red"
                          size={"sm"}
                          onClick={() => DeleteMethod(item.uuid)}
                        >
                          <IconTrash />
                        </ActionIcon>
                      </Tooltip>
                      <Tooltip label="Редактировать (или двойной клик)">
                        <ActionIcon
                          variant="transparent"
                          size={"sm"}
                          onClick={() => EditMethod(item.uuid)}
                        >
                          <IconEdit />
                        </ActionIcon>
                      </Tooltip>
                    </Flex>
                  </Table.Td>
                </Table.Tr>
              ))}
          </Table.Tbody>
        </Table>
      </Box>
    </>
  );
}
